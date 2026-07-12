#ifndef BOMBERMAN_CORE_WORLD_H
#define BOMBERMAN_CORE_WORLD_H

#include <cstddef>
#include <memory>
#include <optional>
#include <string>
#include <vector>
#include "AbstractFactory.h"
#include "EntityModel.h"
#include "Renderer.h"

namespace core {

class World {
private:
    std::shared_ptr<AbstractFactory> factory;

    std::vector<std::unique_ptr<EntityModel>> entities;
    std::optional<std::size_t> playerIndex;

    [[nodiscard]] bool collidesWithBlockingEntity(std::size_t moverIndex, const Vector2& position, const Vector2& size) const;

    [[nodiscard]] bool insideWorldBounds(const Vector2& position, const Vector2& size) const;
public:
    void addEntity(std::unique_ptr<EntityModel> entity);

    void movePlayer(const Vector2& direction, float deltaTime);

    void render(Renderer& renderer) const;

    World(std::shared_ptr<AbstractFactory> factory, const std::string& filename);
};

} // namespace core

#endif // BOMBERMAN_CORE_WORLD_H