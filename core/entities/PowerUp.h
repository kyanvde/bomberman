#ifndef BOMBERMAN_CORE_ENTITIES_POWERUP_H
#define BOMBERMAN_CORE_ENTITIES_POWERUP_H

#include "EntityModel.h"

namespace core {

/**
 * @brief The PowerUp class represents a power-up entity in the game world.
 * It inherits from EntityModel and provides specific behavior for power-up entities.
 */
class PowerUp final : public EntityModel {
public:
    /**
     * @brief Constructs a new PowerUp object with the specified position and size.
     * @param pos The position of the power-up in world coordinates.
     * @param size The size of the power-up in world coordinates.
     */
    PowerUp(const Vector2& pos, const Vector2& size);

    /**
     * @brief A power-up caught in an explosion's blast is destroyed rather than picked up.
     */
    [[nodiscard]] bool isPowerUp() const noexcept override { return true; }
};

}; // namespace core

#endif // BOMBERMAN_CORE_ENTITIES_POWERUP_H