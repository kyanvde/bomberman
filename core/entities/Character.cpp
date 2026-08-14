#include "Character.h"

#include "BasicAIController.h"
#include "World.h"

namespace core {
namespace {
// How much each Skates power-up permanently adds to a character's speed multiplier.
constexpr float skatesSpeedBoost = 0.5f;

// Every non-White character is bot-controlled. Each color gets slightly different tuning on the
// same BasicAIController logic, so the three bots aren't identical without duplicating any
// decision-making code.
std::unique_ptr<AIController> makeAIControllerFor(const CharacterColor color) {
    switch (color) {
    case CharacterColor::Blue:
        return std::make_unique<BasicAIController>(4.f, 6.f);
    case CharacterColor::Red:
        return std::make_unique<BasicAIController>(6.f, 4.f); // more aggressive, less item-hungry
    case CharacterColor::Black:
        return std::make_unique<BasicAIController>(3.f, 8.f); // more cautious, more item-hungry
    case CharacterColor::White:
        return nullptr;
    }
    return nullptr;
}
} // namespace

Character::Character(const Vector2& pos, const Vector2& size, const CharacterColor& color)
    : EntityModel(pos, size), color(color), aiController(makeAIControllerFor(color)) {}

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

void Character::onTick(World& world, const EntityId selfId, const float deltaTime) {
    if (!aiController) {
        return;
    }

    const Decision decision = aiController->decide(world, *this);

    // Placing a bomb before moving means it lands on the tile the decision was made from (e.g.
    // next to a destructible wall), not wherever this tick's movement step ends up.
    if (decision.placeBomb) {
        world.placeBomb(selfId);
    }

    world.moveCharacter(selfId, decision.direction, deltaTime);
}
} // namespace core