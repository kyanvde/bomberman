#ifndef BOMBERMAN_CORE_ENTITIES_BOMB_H
#define BOMBERMAN_CORE_ENTITIES_BOMB_H

#include "EntityModel.h"

namespace core {

/**
 * @brief The Bomb class represents a bomb entity in the game world.
 * It inherits from EntityModel and provides specific behavior for bomb entities.
 */
class Bomb final : public EntityModel {
public:
    /**
     * @brief Constructs a new Bomb object with the specified position and size.
     * @param pos The position of the bomb in world coordinates.
     * @param size The size of the bomb in world coordinates.
     */
    Bomb(const Vector2& pos, const Vector2& size);

    [[nodiscard]] int renderLayer() const noexcept override;
};

}; // namespace core

#endif // BOMBERMAN_CORE_ENTITIES_BOMB_H