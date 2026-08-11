#include "Character.h"

namespace core {
    Character::Character(const Vector2& pos, const Vector2& size) : EntityModel(pos, size) {}

    bool Character::isBlockedBy(const EntityModel& obstacle, const Vector2& moverPosition, const Vector2& moverSize) const {
        return obstacle.blocksCharacterMovement(*this, moverPosition, moverSize);
    }

    void Character::onMovementAttempt(const Vector2 &direction) {
        if (direction.x == 0.f && direction.y == 0.f) {
            setAnimationType(AnimationType::Idle);
            return;
        }

        if (std::abs(direction.x) > std::abs(direction.y)) {
            setAnimationType(direction.x < 0.f ? AnimationType::WalkLeft : AnimationType::WalkRight);
        } else {
            setAnimationType(direction.y < 0.f ? AnimationType::WalkUp : AnimationType::WalkDown);
        }
    }
} // namespace core