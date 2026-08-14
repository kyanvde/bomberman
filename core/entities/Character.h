#ifndef BOMBERMAN_CORE_ENTITIES_CHARACTER_H
#define BOMBERMAN_CORE_ENTITIES_CHARACTER_H

#include "EntityModel.h"

namespace core {

/**
 * @brief The Character class represents a character entity in the game world.
 * It inherits from EntityModel and provides specific behavior for character entities,
 * including player control and collision detection with obstacles.
 */
class Character final : public EntityModel {
public:
    /**
     * @brief Constructs a new Character object with the specified position and size.
     * @param pos The position of the character in world coordinates.
     * @param size The size of the character in world coordinates.
     */
    Character(const Vector2& pos, const Vector2& size);

    /**
     * @brief Checks if the character is player-controlled.
     * @return True, indicating that this character is controlled by the player.
     */
    [[nodiscard]] bool isPlayerControlled() const noexcept override { return true; }

    /**
     * @brief Characters should render above the static world tiles.
     */
    [[nodiscard]] int renderLayer() const noexcept override;

    /**
     * @brief Checks if the character is blocked by a given obstacle at a specified position and size.
     * @param obstacle The obstacle entity to check for collision with the character.
     * @param position The position of the character in world coordinates.
     * @param size The size of the character in world coordinates.
     * @return True if the character is blocked by the obstacle, false otherwise.
     */
    [[nodiscard]] bool isBlockedBy(const EntityModel& obstacle, const Vector2& moverPosition,
                                   const Vector2& moverSize) const override;

    void onMovementAttempt(const Vector2& direction) override;
};

}; // namespace core

#endif // BOMBERMAN_CORE_ENTITIES_CHARACTER_H