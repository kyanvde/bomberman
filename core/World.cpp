#include "World.h"

#include "WorldLoader.h"
#include <cmath>
#include <iostream>

namespace core {
    namespace {
        constexpr float kPlayerSpeed = 10.0f;
    }

    void World::addEntity(std::unique_ptr<EntityModel> entity) {
        if (!playerIndex.has_value() && entity->isPlayerControlled()) {
            playerIndex = entities.size();
        }

        entities.push_back(std::move(entity));
    }

    bool World::collidesWithBlockingEntity(const std::size_t moverIndex, const Vector2& position, const Vector2& size) const {
        const EntityModel& mover = *entities[moverIndex];

        for (std::size_t i = 0; i < entities.size(); ++i) {
            if (i == moverIndex) {
                continue;
            }

            if (entities[i]->blocksMovementOf(mover, position, size)) {
                return true;
            }
        }

        return false;
    }

    bool World::insideWorldBounds(const Vector2& position, const Vector2& size) {
        return position.x >= -1.f && position.y >= -1.f &&
               position.x + size.x <= 1.f && position.y + size.y <= 1.f;
    }

    void World::movePlayer(const Vector2& direction, float deltaTime) const {
        if (!playerIndex.has_value() || deltaTime <= 0.f) {
            return;
        }

        const std::size_t currentPlayerIndex = *playerIndex;
        EntityModel& player = *entities[currentPlayerIndex];

        const float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
        if (length <= 0.f) {
            return;
        }

        const Vector2 normalizedDirection(direction.x / length, direction.y / length);
        const Vector2 delta(normalizedDirection.x * kPlayerSpeed * deltaTime, normalizedDirection.y * kPlayerSpeed * deltaTime);
        const Vector2 size = player.getSize();

        Vector2 nextPosition = player.getPosition();

        const Vector2 movedX(nextPosition.x + delta.x, nextPosition.y);
        if (insideWorldBounds(movedX, size) && !collidesWithBlockingEntity(currentPlayerIndex, movedX, size)) {
            nextPosition.x = movedX.x;
        }

        const Vector2 movedY(nextPosition.x, nextPosition.y + delta.y);
        if (insideWorldBounds(movedY, size) && !collidesWithBlockingEntity(currentPlayerIndex, movedY, size)) {
            nextPosition.y = movedY.y;
        }

        player.setPosition(nextPosition);
    }

    void World::render(AbstractRenderer& renderer) const {
        for (const auto& entity : entities) {
            if (entity) {
                entity->render(renderer);
            }
        }
    }

    World::World(std::shared_ptr<AbstractFactory> factory, const std::string& filename) : factory(factory) {
        WorldLoader::loadFromFile(filename, *this, factory);
    }
} // namespace core