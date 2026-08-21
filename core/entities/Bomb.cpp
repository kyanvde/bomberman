#include "Bomb.h"

#include "Character.h"
#include "Collision.h"
#include "World.h"

#include <algorithm>
#include <cmath>

namespace core {
namespace {
constexpr float bombFuseSeconds = 2.f;

// Tolerance for comparing a tile's coordinate against the bomb's own, matching World's tile-query
// margin convention.
constexpr float tileAlignmentEpsilon = 0.0005f;

// The full, fixed set of character colors this game ever has. Used to scan for bystanders on a
// bomb's tile without needing World to expose "every character here" as its own query.
constexpr CharacterColor allColors[] = {CharacterColor::White, CharacterColor::Blue, CharacterColor::Red,
                                        CharacterColor::Black};
} // namespace

Bomb::Bomb(const Vector2& pos, const Vector2& size, const CharacterColor& owner, const int radius)
    : EntityModel(pos, size), owner(owner), radius(radius), fuseRemaining(bombFuseSeconds), exemptColors{owner} {
    setAnimationType(AnimationType::BombTicking);
    Subject::notify();
}

int Bomb::renderLayer() const noexcept { return 1; }

CharacterColor Bomb::getOwner() const noexcept { return owner; }

int Bomb::getRadius() const noexcept { return radius; }

bool Bomb::isBomb() const noexcept { return true; }

void Bomb::onTick(World& world, const EntityId selfId, const float deltaTime) {
    if (!bystandersScanned) {
        for (const CharacterColor& color : allColors) {
            if (color != owner && world.isTileOccupiedByColor(getPosition(), getSize(), color)) {
                exemptColors.push_back(color);
            }
        }
        bystandersScanned = true;
    }

    // Any exempt color that has since left the tile loses its exemption permanently -- even if it
    // steps back onto the tile later, it's blocked like anyone approaching from outside.
    exemptColors.erase(std::remove_if(exemptColors.begin(), exemptColors.end(),
                                      [&](const CharacterColor& color) {
                                          return !world.isTileOccupiedByColor(getPosition(), getSize(), color);
                                      }),
                       exemptColors.end());

    fuseRemaining -= deltaTime;
    if (fuseRemaining <= 0.f) {
        world.detonateBomb(selfId, radius, owner);
    }
}

bool Bomb::blocksCharacterMovement(const Character& character, const Vector2& characterPosition,
                                   const Vector2& characterSize) const {
    const bool exempt = std::find(exemptColors.begin(), exemptColors.end(), character.getColor()) != exemptColors.end();
    if (exempt) {
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
