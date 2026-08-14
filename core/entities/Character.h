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

    /**
     * @brief The blast radius, in tiles, of bombs this character places. Starts at 1; increased
     * permanently by Fire power-ups.
     */
    int bombRadius = 1;

    /**
     * @brief The maximum number of bombs this character can have active at once. Starts at 1;
     * increased permanently by Extra Bomb power-ups.
     */
    int maxBombs = 1;

    /**
     * @brief How many of this character's own bombs are currently active (placed but not yet
     * exploded/removed).
     */
    int activeBombs = 0;

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
     * @brief Retrieves this character's own color.
     * @return This character's CharacterColor, wrapped in an optional as required by the base
     * class interface.
     */
    [[nodiscard]] std::optional<CharacterColor> getCharacterColor() const noexcept override { return color; }

    /**
     * @brief Checks whether this character currently has a free bomb slot.
     * @return True if activeBombs is below maxBombs, false otherwise.
     */
    [[nodiscard]] bool canPlaceBomb() const noexcept override { return activeBombs < maxBombs; }

    /**
     * @brief Retrieves this character's current bomb blast radius.
     * @return The blast radius, in tiles, that this character's next bomb should have.
     */
    [[nodiscard]] int getBombRadius() const noexcept override { return bombRadius; }

    /**
     * @brief Records that this character has just placed a bomb.
     */
    void onBombPlaced() override { ++activeBombs; }

    /**
     * @brief Records that one of this character's bombs has just exploded (or otherwise been
     * removed), freeing up a bomb slot.
     */
    void onBombExploded() override {
        if (activeBombs > 0) {
            --activeBombs;
        }
    }

    /**
     * @brief A character always dies when an explosion reaches it. (Once a "dead"/"alive" state
     * is introduced in a later phase, this will also need to account for that.)
     */
    [[nodiscard]] bool isKilledByExplosion() const noexcept override { return true; }

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