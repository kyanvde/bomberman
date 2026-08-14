#include "Bomb.h"

#include "Character.h"
#include "Collision.h"
#include "World.h"

namespace core {
namespace {
constexpr float bombFuseSeconds = 2.f;
} // namespace

Bomb::Bomb(const Vector2& pos, const Vector2& size, const CharacterColor& owner, const int radius)
    : EntityModel(pos, size), owner(owner), radius(radius), fuseRemaining(bombFuseSeconds) {
    setAnimationType(AnimationType::BombTicking);
    Subject::notify();
}

int Bomb::renderLayer() const noexcept { return 1; }

void Bomb::onTick(World& world, const EntityId selfId, const float deltaTime) {
    if (!armed && !world.isTileOccupiedByColor(getPosition(), getSize(), owner)) {
        armed = true;
    }

    fuseRemaining -= deltaTime;
    if (fuseRemaining <= 0.f) {
        world.detonateBomb(selfId, radius, owner);
    }
}

bool Bomb::blocksCharacterMovement(const Character& character, const Vector2& characterPosition,
                                   const Vector2& characterSize) const {
    if (!armed && character.getColor() == owner) {
        return false;
    }

    return intersects(characterPosition, characterSize, getPosition(), getSize());
}

void Bomb::detonate(World& world, const EntityId selfId) { world.detonateBomb(selfId, radius, owner); }
} // namespace core
