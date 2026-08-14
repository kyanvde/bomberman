#include "Bomb.h"

#include "Character.h"
#include "Collision.h"
#include "World.h"

#include <cmath>

namespace core {
namespace {
constexpr float bombFuseSeconds = 2.f;

// Tolerance for comparing a tile's coordinate against the bomb's own, matching World's tile-query
// margin convention.
constexpr float tileAlignmentEpsilon = 0.0005f;
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

bool Bomb::threatensTile(const Vector2& tilePosition, const Vector2& tileSize) const noexcept {
    const bool sameRow = std::abs(tilePosition.y - position.y) < tileAlignmentEpsilon;
    const bool sameColumn = std::abs(tilePosition.x - position.x) < tileAlignmentEpsilon;

    if (sameRow) {
        const float distanceInTiles = std::abs(tilePosition.x - position.x) / tileSize.x;
        return distanceInTiles <= static_cast<float>(radius) + tileAlignmentEpsilon;
    }
    if (sameColumn) {
        const float distanceInTiles = std::abs(tilePosition.y - position.y) / tileSize.y;
        return distanceInTiles <= static_cast<float>(radius) + tileAlignmentEpsilon;
    }
    return false;
}
} // namespace core
