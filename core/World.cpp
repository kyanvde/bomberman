#include "World.h"

#include "CharacterColor.h"
#include "Collision.h"
#include "GameEvent.h"
#include "Random.h"
#include "Score.h"
#include "WorldLoader.h"
#include <algorithm>
#include <cmath>

namespace core {
namespace {
constexpr float playerSpeed = 0.75f;
constexpr float collisionMargin = 0.0002f;

// A destroyed destructible wall has a low chance of leaving a power-up behind, on top of the
// grass tile it always leaves.
constexpr double powerUpDropChance = 0.25;

PowerUpType randomPowerUpType() {
    constexpr int typeCount = 3; // Fire, ExtraBomb, Skates
    const int index = std::min(typeCount - 1, static_cast<int>(Random::getInstance().getRandomNumber(0, typeCount)));
    return static_cast<PowerUpType>(index);
}

// Tile-aligned queries (is there a wall/grass/character/etc. "at" this exact tile) compare an
// entity's rectangle against a full-size query tile. Adjacent tiles' edges are computed via
// different floating-point expressions (e.g. a tile's bottom edge as position.y + size.y versus
// the next tile's top edge as -1 + (row+1) * cellSize.y), which are mathematically equal but not
// always bit-identical, so two tiles that only touch can occasionally register as overlapping.
// Insetting the query tile by a tiny margin avoids that boundary-touching false positive while
// still reliably matching a genuine same-tile entity (which always spans the tile fully).
constexpr float tileQueryMargin = 0.0002f;

bool overlapsTile(const Vector2& entityPosition, const Vector2& entitySize, const Vector2& tilePosition,
                  const Vector2& tileSize) {
    const Vector2 insetPosition(tilePosition.x + tileQueryMargin, tilePosition.y + tileQueryMargin);
    const Vector2 insetSize(tileSize.x - tileQueryMargin * 2, tileSize.y - tileQueryMargin * 2);
    return intersects(entityPosition, entitySize, insetPosition, insetSize);
}
} // namespace

Vector2 World::snapToTileTopLeft(const Vector2& position, const Vector2& size) const {
    const Vector2 center(position.x + size.x / 2.f, position.y + size.y / 2.f);

    const float tileX = std::floor((center.x + 1.f) / cellSize.x);
    const float tileY = std::floor((center.y + 1.f) / cellSize.y);

    return Vector2(-1.f + tileX * cellSize.x, -1.f + tileY * cellSize.y);
}

void World::addEntity(std::unique_ptr<EntityModel> entity) {
    entity->setId(nextEntityId++);

    if (!playerId.has_value() && entity->isPlayerControlled()) {
        playerId = entity->getId();
    }

    if (score) {
        entity->attach(score);
    }

    entities.push_back(std::move(entity));
}

std::optional<std::size_t> World::indexOf(const EntityId entityId) const {
    for (std::size_t i = 0; i < entities.size(); ++i) {
        if (entities[i] && entities[i]->getId() == entityId) {
            return i;
        }
    }

    return std::nullopt;
}

bool World::collidesWithBlockingEntity(const std::size_t moverIndex, const Vector2& position,
                                       const Vector2& size) const {
    const EntityModel& mover = *entities[moverIndex];

    const Vector2 collisionSize(size.x - collisionMargin * 2, size.y - collisionMargin * 2);
    const Vector2 collisionPos(position.x + collisionMargin, position.y + collisionMargin);

    for (std::size_t i = 0; i < entities.size(); ++i) {
        if (i == moverIndex) {
            continue;
        }

        if (entities[i]->blocksMovementOf(mover, collisionPos, collisionSize)) {
            return true;
        }
    }

    return false;
}

bool World::insideWorldBounds(const Vector2& position, const Vector2& size) {
    return position.x >= -1.f && position.y >= -1.f && position.x + size.x <= 1.f && position.y + size.y <= 1.f;
}

float World::resolveAxisStep(const std::size_t moverIndex, const Vector2& position, const Vector2& size,
                             const bool horizontal, const float requested) const {
    const auto fits = [&](const float amount) {
        const Vector2 candidate =
            horizontal ? Vector2(position.x + amount, position.y) : Vector2(position.x, position.y + amount);
        return insideWorldBounds(candidate, size) && !collidesWithBlockingEntity(moverIndex, candidate, size);
    };

    if (requested == 0.f || fits(requested)) {
        return requested;
    }

    // Bisect between "definitely fine" and "definitely blocked" to find the contact point. A fixed
    // handful of rounds is plenty: it narrows a frame's travel to well under the margin tile
    // queries already tolerate, so the mover ends up flush rather than a visible gap away.
    constexpr int refinements = 8;
    float safe = 0.f;
    float blocked = requested;

    for (int i = 0; i < refinements; ++i) {
        const float midpoint = (safe + blocked) * 0.5f;
        if (fits(midpoint)) {
            safe = midpoint;
        } else {
            blocked = midpoint;
        }
    }

    return safe;
}

std::optional<Vector2> World::cornerAssistPosition(const std::size_t moverIndex, const Vector2& position,
                                                   const Vector2& size, const Vector2& direction,
                                                   const float maxStep) const {
    // Only for a purely axis-aligned attempt; a diagonal one already resolves per-axis, so at
    // least one of its components got a chance to move on its own.
    const bool vertical = direction.x == 0.f && direction.y != 0.f;
    const bool horizontal = direction.y == 0.f && direction.x != 0.f;
    if (!vertical && !horizontal) {
        return std::nullopt;
    }

    const Vector2 alignedTile = snapToTileTopLeft(position, size);
    const float offset = vertical ? alignedTile.x - position.x : alignedTile.y - position.y;
    if (offset == 0.f) {
        return std::nullopt; // already aligned, so a real wall is blocking -- not misalignment
    }

    // Only assist if the blocked move would actually succeed once aligned. Otherwise the character
    // is walking into a wall head-on and should simply stop, not drift sideways along it.
    const Vector2 alignedAttempt = vertical ? Vector2(alignedTile.x, position.y + direction.y * maxStep)
                                            : Vector2(position.x + direction.x * maxStep, alignedTile.y);
    if (!insideWorldBounds(alignedAttempt, size) || collidesWithBlockingEntity(moverIndex, alignedAttempt, size)) {
        return std::nullopt;
    }

    // Close the misalignment at the character's own speed rather than snapping instantly, so the
    // correction stays smooth, and never overshoot past the aligned coordinate.
    const float step = std::min(std::abs(offset), maxStep);
    const float signedStep = offset > 0.f ? step : -step;
    const Vector2 assisted =
        vertical ? Vector2(position.x + signedStep, position.y) : Vector2(position.x, position.y + signedStep);

    if (!insideWorldBounds(assisted, size) || collidesWithBlockingEntity(moverIndex, assisted, size)) {
        return std::nullopt;
    }

    return assisted;
}

void World::moveCharacter(const EntityId characterId, const Vector2& direction, const float deltaTime) {
    if (deltaTime <= 0.f) {
        return;
    }

    const std::optional<std::size_t> characterIndex = indexOf(characterId);
    if (!characterIndex.has_value()) {
        return;
    }

    EntityModel& character = *entities[*characterIndex];
    if (!character.isAlive()) {
        return; // a dead character no longer responds to movement input
    }

    const float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    if (length <= 0.f) {
        character.onMovementAttempt(Vector2(0.f, 0.f));
        return;
    }

    const Vector2 normalizedDirection(direction.x / length, direction.y / length);
    character.onMovementAttempt(normalizedDirection);

    const float speed = playerSpeed * character.getSpeedMultiplier();
    const Vector2 delta(normalizedDirection.x * speed * deltaTime, normalizedDirection.y * speed * deltaTime);
    const Vector2 size = character.getSize();

    Vector2 nextPosition = character.getPosition();
    const Vector2 previousPosition = nextPosition;

    // Resolved one axis at a time, each as far as it will go, so a blocked direction still lets the
    // other one through and neither is refused outright just because the full step overshoots.
    nextPosition.x += resolveAxisStep(*characterIndex, nextPosition, size, true, delta.x);
    nextPosition.y += resolveAxisStep(*characterIndex, nextPosition, size, false, delta.y);

    // Nothing moved at all: the requested turn may be blocked purely because the character sits
    // slightly off-grid, rather than because a wall genuinely bars the way. Nudge it back into
    // alignment so the turn can open up (see cornerAssistPosition).
    if (nextPosition.x == previousPosition.x && nextPosition.y == previousPosition.y) {
        if (const std::optional<Vector2> assisted =
                cornerAssistPosition(*characterIndex, nextPosition, size, normalizedDirection, speed * deltaTime);
            assisted.has_value()) {
            nextPosition = *assisted;
        }
    }

    character.setPosition(nextPosition);

    // A power-up the character now overlaps is picked up: its stat boost is applied and it is
    // removed. The pending-removal check guards against two characters both picking up the same
    // power-up within the same tick, before the removal is actually flushed.
    for (const auto& entity : entities) {
        if (entity && entity->isPowerUp() && !isPendingRemoval(entity->getId()) &&
            intersects(character.getPosition(), character.getSize(), entity->getPosition(), entity->getSize())) {
            if (const std::optional<PowerUpType> type = entity->getPowerUpType(); type.has_value()) {
                character.applyPowerUp(*type);
            }
            entity->notify(GameEvent{GameEventType::PowerUpCollected,
                                     character.getCharacterColor().value_or(CharacterColor::White)});
            markForRemoval(entity->getId());
        }
    }
}

void World::markForRemoval(const EntityId entityId) {
    if (!hasEntity(entityId) || isPendingRemoval(entityId)) {
        return;
    }

    pendingRemoval.push_back(entityId);
}

bool World::isPendingRemoval(const EntityId entityId) const {
    return std::find(pendingRemoval.begin(), pendingRemoval.end(), entityId) != pendingRemoval.end();
}

void World::flushPendingRemovals() {
    if (pendingRemoval.empty()) {
        return;
    }

    for (const EntityId entityId : pendingRemoval) {
        if (playerId == entityId) {
            playerId.reset();
        }
    }

    entities.erase(std::remove_if(entities.begin(), entities.end(),
                                  [this](const std::unique_ptr<EntityModel>& entity) {
                                      return !entity || std::find(pendingRemoval.begin(), pendingRemoval.end(),
                                                                  entity->getId()) != pendingRemoval.end();
                                  }),
                   entities.end());

    pendingRemoval.clear();
}

void World::update(const float deltaTime) {
    // Snapshot the ids present at the start of this tick, then re-resolve each entity's index
    // fresh on every iteration (rather than holding a raw iterator/reference across the loop).
    // An entity's onTick may itself add entities to `entities` (e.g. an exploding bomb spawning
    // explosion tiles), which can reallocate the vector's storage; re-resolving by id each time
    // means that reallocation never leaves this loop holding a dangling iterator. Entities added
    // during this tick are simply not in the snapshot, so they start ticking on the next update().
    std::vector<EntityId> tickOrder;
    tickOrder.reserve(entities.size());
    for (const auto& entity : entities) {
        if (entity) {
            tickOrder.push_back(entity->getId());
        }
    }

    for (const EntityId id : tickOrder) {
        if (const std::optional<std::size_t> index = indexOf(id); index.has_value()) {
            entities[*index]->onTick(*this, id, deltaTime);
        }
    }

    for (const auto& entity : entities) {
        if (entity) {
            entity->notify();
        }
    }

    flushPendingRemovals();
}

void World::render(AbstractRenderer& renderer) const {
    std::vector<const EntityModel*> renderOrder;
    renderOrder.reserve(entities.size());

    for (const auto& entity : entities) {
        if (entity) {
            renderOrder.push_back(entity.get());
        }
    }

    std::stable_sort(renderOrder.begin(), renderOrder.end(), [](const EntityModel* left, const EntityModel* right) {
        return left->renderLayer() < right->renderLayer();
    });

    for (const EntityModel* entity : renderOrder) {
        entity->render(renderer);
    }
}

World::World(const std::shared_ptr<AbstractFactory>& factory, const std::string& filename,
             const std::shared_ptr<Score>& score)
    : score(score), factory(factory) {
    WorldLoader::loadFromFile(filename, *this, factory);
}

void World::spawnBombAtIndex(const std::size_t entityIndex, const int radius, const CharacterColor& owner) {
    const EntityModel& entity = *entities[entityIndex];
    const Vector2 tilePosition = snapToTileTopLeft(entity.getPosition(), entity.getSize());
    // Route through addEntity (not a direct push_back) so this bomb gets a real, unique id --
    // otherwise it silently keeps its default id of 0, colliding with whichever entity already
    // holds that id.
    addEntity(factory->createBomb(tilePosition, cellSize, owner, radius));
}

void World::placeBomb(const EntityId characterId) {
    const std::optional<std::size_t> characterIndex = indexOf(characterId);
    if (!characterIndex.has_value()) {
        return;
    }

    EntityModel& character = *entities[*characterIndex];
    if (!character.canPlaceBomb()) {
        return;
    }

    character.onBombPlaced();
    spawnBombAtIndex(*characterIndex, character.getBombRadius(),
                     character.getCharacterColor().value_or(CharacterColor::White));
}

bool World::isTileOccupiedByColor(const Vector2& tilePosition, const Vector2& tileSize,
                                  const CharacterColor& color) const {
    for (const auto& entity : entities) {
        if (entity && entity->isCharacterOfColor(color) &&
            overlapsTile(entity->getPosition(), entity->getSize(), tilePosition, tileSize)) {
            return true;
        }
    }

    return false;
}

void World::notifyBombExploded(const CharacterColor& owner) {
    for (const auto& entity : entities) {
        if (entity && entity->isCharacterOfColor(owner)) {
            entity->onBombExploded();
            return;
        }
    }
}

bool World::isWallAt(const Vector2& tilePosition, const Vector2& tileSize) const {
    for (const auto& entity : entities) {
        if (entity && entity->blocksExplosion() &&
            overlapsTile(entity->getPosition(), entity->getSize(), tilePosition, tileSize)) {
            return true;
        }
    }

    return false;
}

bool World::hasGrassAt(const Vector2& tilePosition, const Vector2& tileSize) const {
    for (const auto& entity : entities) {
        if (entity && entity->isGrass() &&
            overlapsTile(entity->getPosition(), entity->getSize(), tilePosition, tileSize)) {
            return true;
        }
    }

    return false;
}

bool World::hasPowerUpAt(const Vector2& tilePosition, const Vector2& tileSize) const {
    for (const auto& entity : entities) {
        if (entity && entity->isPowerUp() &&
            overlapsTile(entity->getPosition(), entity->getSize(), tilePosition, tileSize)) {
            return true;
        }
    }

    return false;
}

int World::getGridWidth() const noexcept {
    return cellSize.x > 0.f ? static_cast<int>(std::lround(2.f / cellSize.x)) : 0;
}

int World::getGridHeight() const noexcept {
    return cellSize.y > 0.f ? static_cast<int>(std::lround(2.f / cellSize.y)) : 0;
}

GridCoord World::toGrid(const Vector2& position, const Vector2& size) const {
    if (cellSize.x <= 0.f || cellSize.y <= 0.f) {
        return GridCoord{0, 0};
    }

    const Vector2 tile = snapToTileTopLeft(position, size);
    return GridCoord{static_cast<int>(std::lround((tile.x + 1.f) / cellSize.x)),
                     static_cast<int>(std::lround((tile.y + 1.f) / cellSize.y))};
}

Vector2 World::toWorldPosition(const GridCoord& coord) const {
    // Deliberately the same expression WorldLoader uses to place tiles, so a coordinate always
    // converts back to exactly the position the entity on that tile was created at.
    return Vector2(-1.f + static_cast<float>(coord.x) * cellSize.x, -1.f + static_cast<float>(coord.y) * cellSize.y);
}

std::vector<GridCoord> World::tilesOverlapping(const Vector2& position, const Vector2& size) const {
    std::vector<GridCoord> tiles;
    if (cellSize.x <= 0.f || cellSize.y <= 0.f) {
        return tiles;
    }

    // Inset by the same margin tile queries use, so merely touching the next tile's edge does not
    // count as being inside it.
    const float minX = position.x + tileQueryMargin;
    const float minY = position.y + tileQueryMargin;
    const float maxX = position.x + size.x - tileQueryMargin;
    const float maxY = position.y + size.y - tileQueryMargin;

    const int firstX = static_cast<int>(std::floor((minX + 1.f) / cellSize.x));
    const int lastX = static_cast<int>(std::floor((maxX + 1.f) / cellSize.x));
    const int firstY = static_cast<int>(std::floor((minY + 1.f) / cellSize.y));
    const int lastY = static_cast<int>(std::floor((maxY + 1.f) / cellSize.y));

    for (int y = firstY; y <= lastY; ++y) {
        for (int x = firstX; x <= lastX; ++x) {
            tiles.push_back(GridCoord{x, y});
        }
    }

    return tiles;
}

TileGrid World::buildTileGrid() const {
    TileGrid grid;
    grid.width = getGridWidth();
    grid.height = getGridHeight();

    const std::size_t cells = static_cast<std::size_t>(grid.width) * static_cast<std::size_t>(grid.height);
    grid.wall.assign(cells, 0);
    grid.destructible.assign(cells, 0);
    grid.bomb.assign(cells, 0);
    grid.powerUp.assign(cells, 0);
    grid.dangerous.assign(cells, 0);

    if (cells == 0) {
        return grid;
    }

    // One pass over the entities marks everything that occupies a tile.
    for (const auto& entity : entities) {
        if (!entity) {
            continue;
        }

        const GridCoord coord = toGrid(entity->getPosition(), entity->getSize());
        if (!grid.inside(coord)) {
            continue;
        }

        const std::size_t cell = grid.index(coord);

        if (entity->blocksExplosion()) {
            grid.wall[cell] = 1;
            if (entity->isDestructibleByExplosion()) {
                grid.destructible[cell] = 1;
            }
        }
        if (entity->isBomb()) {
            grid.bomb[cell] = 1;
        }
        if (entity->isPowerUp()) {
            grid.powerUp[cell] = 1;
        }
        if (const std::optional<CharacterColor> color = entity->getCharacterColor();
            color.has_value() && entity->isAlive()) {
            grid.characters.push_back(CharacterTile{coord, *color});
        }
    }

    // Danger is asked of each bomb rather than derived here, so the grid never has to know what a
    // blast radius is -- only the bomb itself does (EntityModel::threatensTile). There are only
    // ever a handful of bombs, so sweeping the tiles per bomb stays cheap.
    for (const auto& entity : entities) {
        if (!entity || !entity->isBomb()) {
            continue;
        }

        for (int y = 0; y < grid.height; ++y) {
            for (int x = 0; x < grid.width; ++x) {
                const GridCoord coord{x, y};
                const std::size_t cell = grid.index(coord);
                if (grid.dangerous[cell] == 0 && entity->threatensTile(toWorldPosition(coord), cellSize)) {
                    grid.dangerous[cell] = 1;
                }
            }
        }
    }

    return grid;
}

bool World::isBlastStoppedAt(const Vector2& tilePosition, const Vector2& tileSize) const {
    for (const auto& entity : entities) {
        if (entity && entity->blocksExplosion() && !entity->isDestructibleByExplosion() &&
            overlapsTile(entity->getPosition(), entity->getSize(), tilePosition, tileSize)) {
            return true;
        }
    }

    return false;
}

bool World::explodeTile(const Vector2& tilePosition, const Vector2& tileSize, const CharacterColor& owner) {
    addEntity(factory->createExplosion(tilePosition, tileSize));

    bool destroyedWall = false;

    // Snapshot which entities occupy this tile before touching any of them: destroying a wall,
    // killing a character, or chain-detonating a bomb here can itself mutate `entities` (removal
    // marks, and -- once chain reactions recurse -- further explodeTile calls elsewhere), so this
    // follows the same discipline as World::update's tick loop rather than holding a raw iterator
    // across those mutations.
    std::vector<EntityId> idsHere;
    for (const auto& entity : entities) {
        if (entity && overlapsTile(entity->getPosition(), entity->getSize(), tilePosition, tileSize)) {
            idsHere.push_back(entity->getId());
        }
    }

    for (const EntityId id : idsHere) {
        const std::optional<std::size_t> index = indexOf(id);
        if (!index.has_value()) {
            continue; // already removed as a side effect of an earlier entity on this same tile
        }

        EntityModel& entity = *entities[*index];

        if (entity.isDestructibleByExplosion()) {
            const Vector2 wallPosition = entity.getPosition();
            const Vector2 wallSize = entity.getSize();

            markForRemoval(id);
            destroyedWall = true;
            entity.notify(GameEvent{GameEventType::BlockDestroyed, owner});

            const Vector2 abovePosition(wallPosition.x, wallPosition.y - wallSize.y);
            addEntity(factory->createGrass(wallPosition, wallSize, isWallAt(abovePosition, wallSize)));

            if (Random::getInstance().chance(powerUpDropChance)) {
                addEntity(factory->createPowerUp(wallPosition, wallSize, randomPowerUpType()));
            }
        }

        if (entity.isKilledByExplosion()) {
            // Only counts as a kill (for scoring) if the victim isn't the bomb's own owner --
            // blowing yourself up isn't an "enemy killed".
            const std::optional<CharacterColor> victimColor = entity.getCharacterColor();
            entity.onExplosionKill();
            if (!victimColor.has_value() || *victimColor != owner) {
                entity.notify(GameEvent{GameEventType::EntityKilled, owner});
            }
        }

        if (entity.isPowerUp()) {
            markForRemoval(id);
        }

        if (entity.isBomb()) {
            entity.detonate(*this, id);
        }
    }

    return destroyedWall;
}

GameOutcome World::getOutcome() const {
    bool playerFound = false;
    bool playerAlive = false;
    bool anyBotFound = false;
    bool anyBotAlive = false;

    for (const auto& entity : entities) {
        if (!entity) {
            continue;
        }

        const std::optional<CharacterColor> color = entity->getCharacterColor();
        if (!color.has_value()) {
            continue;
        }

        if (*color == CharacterColor::White) {
            playerFound = true;
            playerAlive = entity->isAlive();
        } else {
            anyBotFound = true;
            anyBotAlive = anyBotAlive || entity->isAlive();
        }
    }

    // Player-alive is checked first: if the Player and the last bot die in the same blast, this
    // resolves as a loss rather than a win.
    if (playerFound && !playerAlive) {
        return GameOutcome::PlayerLost;
    }
    if (anyBotFound && !anyBotAlive) {
        return GameOutcome::PlayerWon;
    }
    return GameOutcome::InProgress;
}

void World::detonateBomb(const EntityId bombId, const int radius, const CharacterColor& owner) {
    if (isPendingRemoval(bombId)) {
        return; // already detonated earlier in this same chain reaction
    }

    const std::optional<std::size_t> bombIndex = indexOf(bombId);
    if (!bombIndex.has_value()) {
        return;
    }

    const Vector2 bombTilePosition = entities[*bombIndex]->getPosition();
    const Vector2 tileSize = entities[*bombIndex]->getSize();

    markForRemoval(bombId);
    notifyBombExploded(owner);

    explodeTile(bombTilePosition, tileSize, owner); // the bomb's own tile always explodes

    const Vector2 directions[] = {Vector2(1.f, 0.f), Vector2(-1.f, 0.f), Vector2(0.f, 1.f), Vector2(0.f, -1.f)};

    for (const Vector2& direction : directions) {
        for (int step = 1; step <= radius; ++step) {
            const Vector2 tilePosition(bombTilePosition.x + direction.x * tileSize.x * static_cast<float>(step),
                                       bombTilePosition.y + direction.y * tileSize.y * static_cast<float>(step));

            if (isBlastStoppedAt(tilePosition, tileSize)) {
                break; // an indestructible wall -- the blast never reaches this tile
            }

            if (explodeTile(tilePosition, tileSize, owner)) {
                break; // a destructible wall was destroyed here -- the blast stops after it
            }
        }
    }
}
} // namespace core