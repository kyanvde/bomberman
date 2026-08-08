#include "Character.h"

namespace core {
    Character::Character(const Vector2& pos, const Vector2& size) : EntityModel(pos, size) {}

    bool Character::isBlockedBy(const EntityModel& obstacle, const Vector2& moverPosition, const Vector2& moverSize) const {
        return obstacle.blocksCharacterMovement(*this, moverPosition, moverSize);
    }
} // namespace core