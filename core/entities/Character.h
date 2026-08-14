#ifndef BOMBERMAN_CORE_ENTITIES_CHARACTER_H
#define BOMBERMAN_CORE_ENTITIES_CHARACTER_H

#include "CharacterColor.h"
#include "EntityModel.h"

namespace core {

/**
 * @brief The Character class represents a character entity in the game world.
 * It inherits from EntityModel and provides specific behavior for character entities,
 * including player control and collision detection with obstacles.
 */
class Character final : public EntityModel {
    /**
     * @brief The character's color, which also identifies the human-controlled player (White)
     * versus the computer-controlled bots (Blue/Red/Black).
     */
    CharacterColor color;

public:
    /**
     * @brief Constructs a new Character object with the specified position, size, and color.
     * @param pos The position of the character in world coordinates.
     * @param size The size of the character in world coordinates.
     * @param color The character's color; White identifies the human-controlled player.
     */
    Character(const Vector2& pos, const Vector2& size, const CharacterColor& color);

    /**
     * @brief Retrieves the character's color.
     * @return The CharacterColor this character was constructed with.
     */
    [[nodiscard]] CharacterColor getColor() const noexcept { return color; }

    /**
     * @brief Checks if the character is player-controlled.
     * @return True if this character is the White (human-controlled) character.
     */
    [[nodiscard]] bool isPlayerControlled() const noexcept override { return color == CharacterColor::White; }

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