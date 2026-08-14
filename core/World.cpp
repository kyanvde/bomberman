#include "World.h"

#include "WorldLoader.h"
#include <algorithm>
#include <cmath>

namespace core {
namespace {
constexpr float playerSpeed = 0.75f;
constexpr float collisionMargin = 0.0002f;
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
    if (!hasEntity(entityId)) {
        return;
    }

    if (std::find(pendingRemoval.begin(), pendingRemoval.end(), entityId) == pendingRemoval.end()) {
        pendingRemoval.push_back(entityId);
    }
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

void World::spawnBombAtIndex(const std::size_t entityIndex) {
    const EntityModel& entity = *entities[entityIndex];
    const Vector2 tilePosition = snapToTileTopLeft(entity.getPosition(), entity.getSize());
    entities.push_back(factory->createBomb(tilePosition, cellSize));
}

void World::placeBomb(const EntityId characterId) {
    const std::optional<std::size_t> characterIndex = indexOf(characterId);
    if (characterIndex.has_value()) {
        spawnBombAtIndex(*characterIndex);
    }
}
} // namespace core