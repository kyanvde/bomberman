#ifndef BOMBERMAN_CORE_ENTITIES_BOMB_H
#define BOMBERMAN_CORE_ENTITIES_BOMB_H

#include "CharacterColor.h"
#include "EntityModel.h"

#include <vector>

namespace core {

/**
 * @brief The Bomb class represents a bomb entity in the game world.
 * It inherits from EntityModel and provides specific behavior for bomb entities: a countdown
 * fuse, a blast radius, and collision rules that let anyone standing on its tile the moment it's
 * placed walk off freely, after which it blocks them (including its own owner) like anyone else.
 */
class Bomb final : public EntityModel {
    /**
     * @brief The color of the character that placed this bomb, used for the owner-exception
     * collision rule and (once it explodes) to attribute the blast for scoring.
     */
    CharacterColor owner;

    /**
     * @brief The blast radius of this bomb, in tiles, in each of the four directions.
     */
    int radius;

    /**
     * @brief The time remaining, in seconds, before this bomb explodes.
     */
    float fuseRemaining;

    /**
     * @brief Colors currently exempt from being blocked by this bomb: the owner from
     * construction, plus (once the first onTick has run) anyone else who happened to already be
     * standing on its tile when it was placed. Characters don't block each other's movement, so
     * an enemy can walk onto another character's tile and drop a bomb there -- without this,
     * whoever it landed on would have no grace period at all and be trapped in place until the
     * bomb explodes. Each color is dropped from this list, permanently, the moment it leaves the
     * tile -- exactly mirroring the owner's own original arming rule, just generalized to
     * whoever else the bomb happened to land on.
     */
    std::vector<CharacterColor> exemptColors;

    /**
     * @brief Whether the one-time bystander scan (see exemptColors) has run yet. Bystanders can
     * only be discovered via a World query, which isn't available at construction time, so this
     * defers that scan to the bomb's first onTick; the owner's own exemption is already in place
     * from construction and unaffected by this.
     */
    bool bystandersScanned = false;

public:
    /**
     * @brief Constructs a new Bomb object with the specified position, size, owner, and radius.
     * @param pos The position of the bomb in world coordinates.
     * @param size The size of the bomb in world coordinates.
     * @param owner The color of the character that placed this bomb.
     * @param radius The blast radius of this bomb, in tiles, in each direction.
     */
    Bomb(const Vector2& pos, const Vector2& size, const CharacterColor& owner = CharacterColor::White, int radius = 1);

    [[nodiscard]] int renderLayer() const noexcept override;

    /**
     * @brief Retrieves the color of the character that placed this bomb.
     * @return The owner's CharacterColor.
     */
    [[nodiscard]] CharacterColor getOwner() const noexcept { return owner; }

    /**
     * @brief Retrieves this bomb's blast radius.
     * @return The blast radius, in tiles, in each direction.
     */
    [[nodiscard]] int getRadius() const noexcept { return radius; }

    /**
     * @brief On the first call, finds any bystanders already standing on this bomb's tile and
     * grants them the same exemption as the owner. Every call then drops any exempt color that
     * has since left the tile. Finally counts down the fuse; once it expires, triggers this
     * bomb's own explosion via World::detonateBomb.
     * @param world The world this bomb belongs to.
     * @param selfId This bomb's own identifier.
     * @param deltaTime The time elapsed since the previous tick, in seconds.
     */
    void onTick(World& world, EntityId selfId, float deltaTime) override;

    /**
     * @brief Checks if this bomb blocks the movement of a character at a given position and size.
     * Does not block a character whose color is still exempt (see exemptColors).
     * @param character The character to check for collision with the bomb.
     * @param characterPosition The position of the character in world coordinates.
     * @param characterSize The size of the character in world coordinates.
     * @return True if the bomb blocks the character's movement, false otherwise.
     */
    [[nodiscard]] bool blocksCharacterMovement(const Character& character, const Vector2& characterPosition,
                                               const Vector2& characterSize) const override;

    /**
     * @brief A bomb caught in another explosion's blast should itself chain-detonate.
     */
    [[nodiscard]] bool isBomb() const noexcept override { return true; }

    /**
     * @brief Checks whether this bomb's eventual cross-shaped blast (own tile plus up to radius
     * tiles in each of the four directions) would reach the given tile. Deliberately ignores
     * whether the bomb is armed yet, and whether an indestructible wall would actually stop the
     * blast before reaching that far -- a cheap, slightly conservative danger estimate is safer
     * for AI purposes than an exact one.
     * @param tilePosition The top-left corner of the tile to check.
     * @param tileSize The size of the tile to check.
     * @return True if this bomb's blast would reach the given tile, false otherwise.
     */
    [[nodiscard]] bool threatensTile(const Vector2& tilePosition, const Vector2& tileSize) const noexcept override;

    /**
     * @brief Triggers this bomb's own explosion immediately, using its own radius and owner.
     * Called by World when this bomb is caught in another explosion's blast (a chain reaction).
     * @param world The world this bomb belongs to.
     * @param selfId This bomb's own identifier.
     */
    void detonate(World& world, EntityId selfId) override;
};

}; // namespace core

#endif // BOMBERMAN_CORE_ENTITIES_BOMB_H
