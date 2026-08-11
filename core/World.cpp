#include "World.h"

#include "WorldLoader.h"
#include <algorithm>
#include <cmath>

#include "CharacterColor.h"

namespace core {
    namespace {
        constexpr float playerSpeed = 0.75f;
        constexpr float collisionMargin = 0.0002f;
    }

    Vector2 World::snapToTileTopLeft(const Vector2 &position, const Vector2 &size) const {
        const Vector2 center(position.x + size.x / 2.f, position.y + size.y / 2.f);

        const float tileX = std::floor((center.x + 1.f) / cellSize.x);
        const float tileY = std::floor((center.y + 1.f) / cellSize.y);

        return Vector2(-1.f + tileX * cellSize.x, -1.f + tileY * cellSize.y);
    }

    void World::addEntity(std::unique_ptr<EntityModel> entity) {
        if (!playerIndex.has_value() && entity->isPlayerControlled()) {
            playerIndex = entities.size();
        }

        entities.push_back(std::move(entity));
    }

    bool World::collidesWithBlockingEntity(const std::size_t moverIndex, const Vector2& position, const Vector2& size) const {
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
        return position.x >= -1.f && position.y >= -1.f &&
               position.x + size.x <= 1.f && position.y + size.y <= 1.f;
    }

    void World::movePlayer(const Vector2& direction, const float deltaTime) const {
        if (!playerIndex.has_value() || deltaTime <= 0.f) {
            return;
        }

        const std::size_t currentPlayerIndex = *playerIndex;
        EntityModel& player = *entities[currentPlayerIndex];

        const float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
        if (length <= 0.f) {
            player.onMovementAttempt(Vector2(0.f, 0.f));
            return;
        }

        const Vector2 normalizedDirection(direction.x / length, direction.y / length);
        player.onMovementAttempt(normalizedDirection);

        const Vector2 delta(normalizedDirection.x * playerSpeed * deltaTime, normalizedDirection.y * playerSpeed * deltaTime);
        const Vector2 size = player.getSize();

        Vector2 nextPosition = player.getPosition();

        const Vector2 movedX(nextPosition.x + delta.x, nextPosition.y);
        const bool canMoveX = insideWorldBounds(movedX, size) && !collidesWithBlockingEntity(currentPlayerIndex, movedX, size);

        const Vector2 movedY(nextPosition.x, nextPosition.y + delta.y);
        const bool canMoveY = insideWorldBounds(movedY, size) && !collidesWithBlockingEntity(currentPlayerIndex, movedY, size);

        if (canMoveX && canMoveY) {
            nextPosition.x = movedX.x;
            nextPosition.y = movedY.y;
        } else if (canMoveX) {
            nextPosition.x = movedX.x;
        } else if (canMoveY) {
            nextPosition.y = movedY.y;
        }

        player.setPosition(nextPosition);
    }

    void World::update() {
        for (const auto& entity : entities) {
            if (entity) {
                entity->notify();
            }
        }
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

    World::World(const std::shared_ptr<AbstractFactory>& factory, const std::string& filename)
    : factory(factory) {
        WorldLoader::loadFromFile(filename, *this, factory);
    }

    void World::spawnBombAt(const std::size_t entityIndex) {
        const EntityModel& entity = *entities[entityIndex];
        const Vector2 tilePosition = snapToTileTopLeft(entity.getPosition(), entity.getSize());
        entities.push_back(factory->createBomb(tilePosition, cellSize));
    }

    void World::spawnBomb(const CharacterColor &color) {
        if (color == CharacterColor::White && playerIndex.has_value()) {
            spawnBombAt(*playerIndex);
        }
    }
} // namespace core