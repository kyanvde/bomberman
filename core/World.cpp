#include "World.h"

#include "CharacterColor.h"
#include "Collision.h"
#include "Random.h"
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

void World::moveCharacter(const EntityId characterId, const Vector2& direction, const float deltaTime) {
    if (deltaTime <= 0.f) {
        return;
    }

    const std::optional<std::size_t> characterIndex = indexOf(characterId);
    if (!characterIndex.has_value()) {
        return;
    }

    EntityModel& character = *entities[*characterIndex];

    const float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    if (length <= 0.f) {
        character.onMovementAttempt(Vector2(0.f, 0.f));
        return;
    }

    const Vector2 normalizedDirection(direction.x / length, direction.y / length);
    character.onMovementAttempt(normalizedDirection);

    const Vector2 delta(normalizedDirection.x * playerSpeed * deltaTime,
                        normalizedDirection.y * playerSpeed * deltaTime);
    const Vector2 size = character.getSize();

    Vector2 nextPosition = character.getPosition();

    const Vector2 movedX(nextPosition.x + delta.x, nextPosition.y);
    const bool canMoveX = insideWorldBounds(movedX, size) && !collidesWithBlockingEntity(*characterIndex, movedX, size);

    const Vector2 movedY(nextPosition.x, nextPosition.y + delta.y);
    const bool canMoveY = insideWorldBounds(movedY, size) && !collidesWithBlockingEntity(*characterIndex, movedY, size);

    if (canMoveX && canMoveY) {
        nextPosition.x = movedX.x;
        nextPosition.y = movedY.y;
    } else if (canMoveX) {
        nextPosition.x = movedX.x;
    } else if (canMoveY) {
        nextPosition.y = movedY.y;
    }

    character.setPosition(nextPosition);
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

World::World(const std::shared_ptr<AbstractFactory>& factory, const std::string& filename) : factory(factory) {
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

bool World::isBlastStoppedAt(const Vector2& tilePosition, const Vector2& tileSize) const {
    for (const auto& entity : entities) {
        if (entity && entity->blocksExplosion() && !entity->isDestructibleByExplosion() &&
            overlapsTile(entity->getPosition(), entity->getSize(), tilePosition, tileSize)) {
            return true;
        }
    }

    return false;
}

bool World::explodeTile(const Vector2& tilePosition, const Vector2& tileSize) {
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

            const Vector2 abovePosition(wallPosition.x, wallPosition.y - wallSize.y);
            addEntity(factory->createGrass(wallPosition, wallSize, isWallAt(abovePosition, wallSize)));

            if (Random::getInstance().chance(powerUpDropChance)) {
                addEntity(factory->createPowerUp(wallPosition, wallSize, randomPowerUpType()));
            }
        }

        if (entity.isKilledByExplosion()) {
            // TODO (death/win-lose phase): transition to a "dead" state instead of removing.
            markForRemoval(id);
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

    explodeTile(bombTilePosition, tileSize); // the bomb's own tile always explodes

    const Vector2 directions[] = {Vector2(1.f, 0.f), Vector2(-1.f, 0.f), Vector2(0.f, 1.f), Vector2(0.f, -1.f)};

    for (const Vector2& direction : directions) {
        for (int step = 1; step <= radius; ++step) {
            const Vector2 tilePosition(bombTilePosition.x + direction.x * tileSize.x * static_cast<float>(step),
                                       bombTilePosition.y + direction.y * tileSize.y * static_cast<float>(step));

            if (isBlastStoppedAt(tilePosition, tileSize)) {
                break; // an indestructible wall -- the blast never reaches this tile
            }

            if (explodeTile(tilePosition, tileSize)) {
                break; // a destructible wall was destroyed here -- the blast stops after it
            }
        }
    }
}
} // namespace core