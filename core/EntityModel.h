#ifndef BOMBERMAN_CORE_ENTITYMODEL_H
#define BOMBERMAN_CORE_ENTITYMODEL_H

#include "Subject.h"
#include "Vector2.h"

namespace core {

class Character;

class EntityModel : public Subject {
protected:
    Vector2 position;
    Vector2 size;
public:
    EntityModel(const Vector2& pos, const Vector2& size) : position(pos), size(size) {}

    virtual ~EntityModel() = default;

    [[nodiscard]] const Vector2& getPosition() const noexcept { return position; }

    [[nodiscard]] const Vector2& getSize() const noexcept { return size; }

    void setPosition(const Vector2& newPosition) {
        position = newPosition;
        notify();
    }

    [[nodiscard]] virtual bool isPlayerControlled() const noexcept { return false; }

    [[nodiscard]] virtual bool blocksMovementOf(const EntityModel& mover, const Vector2& moverPosition, const Vector2& moverSize) const {
        return mover.isBlockedBy(*this, moverPosition, moverSize);
    }

    [[nodiscard]] virtual bool isBlockedBy(const EntityModel&, const Vector2&, const Vector2&) const { return false; }

    [[nodiscard]] virtual bool blocksCharacterMovement(const Character&, const Vector2&, const Vector2&) const { return false; }
};

} // namespace core

#endif // BOMBERMAN_CORE_ENTITYMODEL_H