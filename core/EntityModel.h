#ifndef BOMBERMAN_CORE_ENTITYMODEL_H
#define BOMBERMAN_CORE_ENTITYMODEL_H

#include "Subject.h"
#include "Vector2.h"

namespace core {

class Character;

/**
 * @brief The EntityModel class represents a generic entity in the game world.
 * It serves as a base class for specific entity types, such as characters, walls, and power-ups.
 * The class provides basic properties like position and size, and defines methods for movement and collision detection.
 */
class EntityModel : public Subject {
protected:
    /**
     * @brief The position of the entity in world coordinates.
     */
    Vector2 position;

    /**
     * @brief The size of the entity in world coordinates.
     */
    Vector2 size;
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
     * @brief Determines if the entity blocks the movement of another entity.
     * @param mover The entity attempting to move.
     * @param moverPosition The position of the moving entity.
     * @param moverSize The size of the moving entity.
     * @return True if this entity blocks the movement of the mover, false otherwise.
     */
    [[nodiscard]] virtual bool blocksMovementOf(const EntityModel& mover, const Vector2& moverPosition, const Vector2& moverSize) const {
        return mover.isBlockedBy(*this, moverPosition, moverSize);
    }

    /**
     * @brief Determines if this entity blocks the movement of another entity based on their positions and sizes.
     * @param other The other entity to check against.
     * @param otherPosition The position of the other entity.
     * @param otherSize The size of the other entity.
     * @return True if this entity blocks the movement of the other entity, false otherwise.
     */
    [[nodiscard]] virtual bool isBlockedBy(const EntityModel& other, const Vector2& otherPosition, const Vector2& otherSize) const { return false; }

    /**
     * @brief Determines if this entity blocks the movement of a character based on their positions and sizes.
     * @param character The character to check against.
     * @param characterPosition The position of the character.
     * @param characterSize The size of the character.
     * @return True if this entity blocks the movement of the character, false otherwise.
     */
    [[nodiscard]] virtual bool blocksCharacterMovement(const Character& character, const Vector2& characterPosition, const Vector2& characterSize) const { return false; }
};

} // namespace core

#endif // BOMBERMAN_CORE_ENTITYMODEL_H