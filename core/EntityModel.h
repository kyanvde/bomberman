#ifndef BOMBERMAN_CORE_ENTITYMODEL_H
#define BOMBERMAN_CORE_ENTITYMODEL_H

#include "Animation.h"
#include "Subject.h"
#include "Vector2.h"

#include <cstddef>
#include <optional>

namespace core {

class Character;
class World;
enum class CharacterColor;

/**
 * @brief A stable identifier for an entity within its World, assigned once when the entity
 * is added and unaffected by later additions or removals of other entities.
 */
using EntityId = std::size_t;

/**
 * @brief The EntityModel class represents a generic entity in the game world.
 * It serves as a base class for specific entity types, such as characters, walls, and power-ups.
 * The class provides basic properties like position and size, and defines methods for movement and collision detection.
 */
class EntityModel : public Subject {
protected:
    /**
     * @brief The entity's stable identifier. Set once by World::addEntity when the entity is added.
     */
    EntityId id = 0;

    /**
     * @brief The position of the entity in world coordinates.
     */
    Vector2 position;

    /**
     * @brief The size of the entity in world coordinates.
     */
    Vector2 size;

    /**
     * @brief The current animation type of the entity (default is Idle).
     */
    AnimationType animationType = AnimationType::Idle;

    /**
     * @brief Sets the activate animation type of the entity.
     * @param newAnimationType The new AnimationType of the entity.
     */
    void setAnimationType(const AnimationType newAnimationType) {
        this->animationType = newAnimationType;
        notify();
    }

public:
    /**
     * @brief Constructs a new EntityModel object with the specified position and size.
     * @param pos The position of the entity in world coordinates.
     * @param size The size of the entity in world coordinates.
     */
    EntityModel(const Vector2& pos, const Vector2& size) : position(pos), size(size) {}

    /**
     * @brief Virtual destructor for the EntityModel class.
     */
    ~EntityModel() override = default;

    /**
     * @brief Retrieves the position of the entity.
     * @return A constant reference to the Vector2 representing the entity's position.
     */
    [[nodiscard]] const Vector2& getPosition() const noexcept { return position; }

    /**
     * @brief Retrieves the entity's stable identifier.
     * @return The EntityId assigned to this entity when it was added to the World.
     */
    [[nodiscard]] EntityId getId() const noexcept { return id; }

    /**
     * @brief Sets the entity's stable identifier. Called once by World::addEntity.
     * @param newId The identifier assigned to this entity.
     */
    void setId(const EntityId newId) noexcept { id = newId; }

    /**
     * @brief Retrieves the size of the entity.
     * @return A constant reference to the Vector2 representing the entity's size.
     */
    [[nodiscard]] const Vector2& getSize() const noexcept { return size; }

    /**
     * @brief Sets the position of the entity and notifies observers of the change.
     * @param newPosition The new position of the entity in world coordinates.
     */
    void setPosition(const Vector2& newPosition) {
        position = newPosition;
        notify();
    }

    /**
     * @brief Determines if the entity is controlled by the player.
     * @return True if the entity is player-controlled, false otherwise.
     */
    [[nodiscard]] virtual bool isPlayerControlled() const noexcept { return false; }

    /**
     * @brief Retrieves this entity's character color, if it is a character. Only Character
     * overrides this meaningfully. Lets other code (e.g. World resolving who to attribute a
     * placed bomb to, or a bomb checking whether its owner still stands on its tile) ask a
     * generic EntityModel for its color without downcasting to a concrete entity type.
     * @return The entity's CharacterColor, or std::nullopt if it isn't a character.
     */
    [[nodiscard]] virtual std::optional<CharacterColor> getCharacterColor() const noexcept { return std::nullopt; }

    /**
     * @brief Determines if this entity is a character of the given color. Implemented in terms
     * of getCharacterColor(), so only Character (via that override) can ever answer true.
     * @param color The color to compare against.
     * @return True if this entity is a Character of the given color, false otherwise.
     */
    [[nodiscard]] bool isCharacterOfColor(const CharacterColor& color) const noexcept {
        return getCharacterColor() == color;
    }

    /**
     * @brief Determines whether this entity currently has a free bomb slot. Always false except
     * for Character, which overrides it based on its own bomb capacity and how many of its
     * bombs are currently active.
     * @return True if this entity can place another bomb right now, false otherwise.
     */
    [[nodiscard]] virtual bool canPlaceBomb() const noexcept { return false; }

    /**
     * @brief Retrieves this entity's current bomb blast radius. Meaningless for entities that
     * cannot place bombs; only Character overrides it meaningfully.
     * @return The blast radius, in tiles, that a bomb placed by this entity should have.
     */
    [[nodiscard]] virtual int getBombRadius() const noexcept { return 1; }

    /**
     * @brief Notifies this entity that it has just placed a bomb, so it can track how many of
     * its bombs are currently active. Default implementation does nothing.
     */
    virtual void onBombPlaced() {}

    /**
     * @brief Notifies this entity that one of its previously placed bombs has just exploded (or
     * otherwise been removed), freeing up a bomb slot. Default implementation does nothing.
     */
    virtual void onBombExploded() {}

    /**
     * @brief Returns the rendering layer for this entity.
     * Entities with a higher layer are drawn later.
     */
    [[nodiscard]] virtual int renderLayer() const noexcept { return 0; }

    /**
     * @brief Determines if the entity blocks the movement of another entity.
     * @param mover The entity attempting to move.
     * @param moverPosition The position of the moving entity.
     * @param moverSize The size of the moving entity.
     * @return True if this entity blocks the movement of the mover, false otherwise.
     */
    [[nodiscard]] virtual bool blocksMovementOf(const EntityModel& mover, const Vector2& moverPosition,
                                                const Vector2& moverSize) const {
        return mover.isBlockedBy(*this, moverPosition, moverSize);
    }

    /**
     * @brief Determines if this entity blocks the movement of another entity based on their positions and sizes.
     * @param other The other entity to check against.
     * @param otherPosition The position of the other entity.
     * @param otherSize The size of the other entity.
     * @return True if this entity blocks the movement of the other entity, false otherwise.
     */
    [[nodiscard]] virtual bool isBlockedBy(const EntityModel&, const Vector2&, const Vector2&) const { return false; }

    /**
     * @brief Determines if this entity blocks the movement of a character based on their positions and sizes.
     * @param character The character to check against.
     * @param characterPosition The position of the character.
     * @param characterSize The size of the character.
     * @return True if this entity blocks the movement of the character, false otherwise.
     */
    [[nodiscard]] virtual bool blocksCharacterMovement(const Character&, const Vector2&, const Vector2&) const {
        return false;
    }

    /**
     * @brief Called when movement in a given direction is attempted, so the entity
     * can update its animation state accordingly. Default implementation does nothing.
     * @param direction The attempted movement direction (not necessarily normalized to a successful move).
     */
    virtual void onMovementAttempt(const Vector2&) {}

    /**
     * @brief Returns the active animation type of the entity.
     * @return The active AnimationType of the entity.
     */
    [[nodiscard]] AnimationType getAnimationType() const { return animationType; }

    /**
     * @brief Called once per simulation tick so the entity can perform its own time-based
     * self-maintenance (e.g. a bomb counting down its fuse). Default implementation does nothing.
     * May freely query and mutate world (e.g. to detonate, spawn, or remove entities); it must
     * not assume any particular entity storage layout, since world may reallocate internally
     * as a result of this call.
     * @param world The world this entity belongs to.
     * @param selfId This entity's own identifier, for convenience when calling back into world.
     * @param deltaTime The time elapsed since the previous tick, in seconds.
     */
    virtual void onTick(World&, EntityId, float) {}

    /**
     * @brief Determines whether this entity blocks an explosion from propagating past it (the
     * explosion still affects this entity's own tile, but does not continue beyond it). Always
     * false except for Wall, which overrides it to true for both destructible and indestructible
     * walls.
     * @return True if an explosion cannot propagate past this entity, false otherwise.
     */
    [[nodiscard]] virtual bool blocksExplosion() const noexcept { return false; }

    /**
     * @brief Determines whether this entity is destroyed when an explosion reaches it. Always
     * false except for a destructible Wall.
     * @return True if this entity should be destroyed by an explosion that reaches it.
     */
    [[nodiscard]] virtual bool isDestructibleByExplosion() const noexcept { return false; }

    /**
     * @brief Determines whether this entity dies when an explosion reaches it. Always false
     * except for Character.
     * @return True if this entity should die from an explosion that reaches it.
     */
    [[nodiscard]] virtual bool isKilledByExplosion() const noexcept { return false; }

    /**
     * @brief Determines whether this entity is a bomb, so an explosion reaching it should
     * chain-trigger its own detonation. Always false except for Bomb.
     * @return True if this entity is a bomb, false otherwise.
     */
    [[nodiscard]] virtual bool isBomb() const noexcept { return false; }

    /**
     * @brief Determines whether this entity is a power-up, so it should be destroyed (without
     * being picked up) when an explosion reaches it. Always false except for PowerUp.
     * @return True if this entity is a power-up, false otherwise.
     */
    [[nodiscard]] virtual bool isPowerUp() const noexcept { return false; }

    /**
     * @brief Determines whether this entity is grass (open, walkable ground). Always false except
     * for Grass.
     * @return True if this entity is grass, false otherwise.
     */
    [[nodiscard]] virtual bool isGrass() const noexcept { return false; }

    /**
     * @brief Tells this entity to detonate immediately, if it is a bomb caught in another
     * explosion's blast (a chain reaction). Default implementation does nothing. Only Bomb
     * overrides this, using its own privately-held radius and owner to trigger its explosion via
     * World::detonateBomb -- this avoids World ever needing to downcast a generic EntityModel to
     * extract bomb-specific data.
     * @param world The world this entity belongs to.
     * @param selfId This entity's own identifier.
     */
    virtual void detonate(World&, EntityId) {}
};

} // namespace core

#endif // BOMBERMAN_CORE_ENTITYMODEL_H