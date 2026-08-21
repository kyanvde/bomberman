#ifndef BOMBERMAN_CORE_ENTITIES_WALL_H
#define BOMBERMAN_CORE_ENTITIES_WALL_H

#include "EntityModel.h"

namespace core {

/**
 * @brief The Wall class represents a wall entity in the game world.
 * It inherits from EntityModel and provides specific behavior for wall entities,
 * including collision detection with characters.
 */
class Wall final : public EntityModel {
    /**
     * @brief Whether this wall can be destroyed by an explosion.
     */
    bool destructible;

public:
    /**
     * @brief Constructs a new Wall object with the specified position, size, and destructibility.
     * @param pos The position of the wall in world coordinates.
     * @param size The size of the wall in world coordinates.
     * @param destructible A boolean indicating whether the wall is destructible (default is false).
     */
    Wall(const Vector2& pos, const Vector2& size, bool destructible = false);

    /**
     * @brief Checks whether this wall can be destroyed by an explosion.
     * @return True if the wall is destructible, false otherwise.
     */
    [[nodiscard]] bool isDestructible() const noexcept;

    /**
     * @brief Both destructible and indestructible walls stop an explosion from propagating past
     * them.
     */
    [[nodiscard]] bool blocksExplosion() const noexcept override;

    /**
     * @brief Only a destructible wall is destroyed when an explosion reaches it.
     */
    [[nodiscard]] bool isDestructibleByExplosion() const noexcept override;

    /**
     * @brief Checks if the wall blocks the movement of a character at a given position and size.
     * @param character The character to check for collision with the wall.
     * @param position The position of the character in world coordinates.
     * @param size The size of the character in world coordinates.
     * @return True if the wall blocks the character's movement, false otherwise.
     */
    [[nodiscard]] bool blocksCharacterMovement(const Character&, const Vector2& characterPosition,
                                               const Vector2& characterSize) const override;
};

}; // namespace core

#endif // BOMBERMAN_CORE_ENTITIES_WALL_H