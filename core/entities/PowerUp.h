#ifndef BOMBERMAN_CORE_ENTITIES_POWERUP_H
#define BOMBERMAN_CORE_ENTITIES_POWERUP_H

#include "EntityModel.h"
#include "PowerUpType.h"

namespace core {

/**
 * @brief The PowerUp class represents a power-up entity in the game world.
 * It inherits from EntityModel and provides specific behavior for power-up entities.
 */
class PowerUp final : public EntityModel {
    /**
     * @brief Which permanent stat boost this power-up grants when picked up.
     */
    PowerUpType type;

public:
    /**
     * @brief Constructs a new PowerUp object with the specified position, size, and type.
     * @param pos The position of the power-up in world coordinates.
     * @param size The size of the power-up in world coordinates.
     * @param type Which permanent stat boost this power-up grants when picked up.
     */
    PowerUp(const Vector2& pos, const Vector2& size, PowerUpType type);

    /**
     * @brief Retrieves which permanent stat boost this power-up grants.
     * @return This power-up's PowerUpType.
     */
    [[nodiscard]] PowerUpType getType() const noexcept { return type; }

    /**
     * @brief A power-up caught in an explosion's blast is destroyed rather than picked up.
     */
    [[nodiscard]] bool isPowerUp() const noexcept override { return true; }

    /**
     * @brief Retrieves this power-up's type, so a character picking it up can apply the right
     * stat boost without downcasting.
     */
    [[nodiscard]] std::optional<PowerUpType> getPowerUpType() const noexcept override { return type; }
};

}; // namespace core

#endif // BOMBERMAN_CORE_ENTITIES_POWERUP_H