#include "Character.h"

namespace core {
    Character::Character(const Vector2& pos, const Vector2& size) : EntityModel(pos, size) {}

    bool Character::isBlockedBy(const EntityModel& obstacle, const Vector2& position, const Vector2& size) const {
        return obstacle.blocksCharacterMovement(*this, position, size);
    }
} // namespace core