#include "Character.h"

namespace core {
namespace {
// How much each Skates power-up permanently adds to a character's speed multiplier.
constexpr float skatesSpeedBoost = 0.5f;
} // namespace

Character::Character(const Vector2& pos, const Vector2& size, const CharacterColor& color)
    : EntityModel(pos, size), color(color) {}

void Character::applyPowerUp(const PowerUpType type) {
    switch (type) {
    case PowerUpType::Fire:
        ++bombRadius;
        break;
    case PowerUpType::ExtraBomb:
        ++maxBombs;
        break;
    case PowerUpType::Skates:
        speedMultiplier += skatesSpeedBoost;
        break;
    }
}

bool Character::isBlockedBy(const EntityModel& obstacle, const Vector2& moverPosition, const Vector2& moverSize) const {
    return obstacle.blocksCharacterMovement(*this, moverPosition, moverSize);
}

void Character::onMovementAttempt(const Vector2& direction) {
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

int Character::renderLayer() const noexcept { return 2; }
} // namespace core