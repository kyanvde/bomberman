#include "BasicAIController.h"

#include "CharacterColor.h"
#include "EntityModel.h"
#include "World.h"

#include <algorithm>
#include <cmath>
#include <optional>

namespace core {
namespace {

float squaredDistance(const Vector2& a, const Vector2& b) {
    const float dx = a.x - b.x;
    const float dy = a.y - b.y;
    return dx * dx + dy * dy;
}

// Picks a single cardinal direction (never diagonal) toward a target position, favoring whichever
// axis has the larger remaining distance -- matches the classic Bomberman grid-movement feel.
Vector2 towards(const Vector2& from, const Vector2& to) {
    const float dx = to.x - from.x;
    const float dy = to.y - from.y;

    if (std::abs(dx) > std::abs(dy)) {
        return Vector2(dx > 0.f ? 1.f : -1.f, 0.f);
    }
    if (dy != 0.f) {
        return Vector2(0.f, dy > 0.f ? 1.f : -1.f);
    }
    return Vector2(0.f, 0.f);
}

const Vector2 cardinalDirections[] = {Vector2(1.f, 0.f), Vector2(-1.f, 0.f), Vector2(0.f, 1.f), Vector2(0.f, -1.f)};

} // namespace

BasicAIController::BasicAIController(const float aggressionRadiusTiles, const float powerUpSearchRadiusTiles)
    : aggressionRadiusTiles(aggressionRadiusTiles), powerUpSearchRadiusTiles(powerUpSearchRadiusTiles) {}

Decision BasicAIController::decide(const World& world, const EntityModel& self) const {
    const Vector2 tileSize = world.getCellSize();
    const Vector2 selfTile = world.tileOf(self.getPosition(), self.getSize());
    const CharacterColor color = self.getCharacterColor().value_or(CharacterColor::White);

    // 1. Flee imminent bomb danger above all else: step toward the first neighboring tile that is
    //    both open and safe. Since danger is derived from every armed bomb's own radius, this
    //    naturally accounts for a bigger blast radius (the bot's own, or an enemy's) requiring a
    //    longer retreat, without hardcoding any particular radius.
    if (world.isTileDangerous(selfTile, tileSize)) {
        for (const Vector2& direction : cardinalDirections) {
            const Vector2 candidateTile(selfTile.x + direction.x * tileSize.x, selfTile.y + direction.y * tileSize.y);
            if (!world.isWallAt(candidateTile, tileSize) && !world.isTileDangerous(candidateTile, tileSize)) {
                return Decision{direction, false};
            }
        }
        return Decision{Vector2(0.f, 0.f), false}; // nowhere safe to go this tick
    }

    // 2. Attack a nearby enemy, or any enemy at all once there is nothing left to break.
    if (const std::optional<Vector2> enemyPosition = world.findNearestEnemyPosition(self.getPosition(), color);
        enemyPosition.has_value()) {
        const float distanceTiles =
            std::sqrt(squaredDistance(self.getPosition(), *enemyPosition)) / std::max(tileSize.x, tileSize.y);

        if (!world.anyDestructibleWallsRemain() || distanceTiles <= aggressionRadiusTiles) {
            const bool adjacent = distanceTiles <= 1.5f;
            return Decision{towards(self.getPosition(), *enemyPosition), adjacent && self.canPlaceBomb()};
        }
    }

    // 3. Bomb an adjacent destructible wall, then retreat from it (placing first, at the current
    //    tile, so the bomb lands on the wall's tile rather than wherever the retreat step ends).
    for (const Vector2& direction : cardinalDirections) {
        const Vector2 adjacentTile(selfTile.x + direction.x * tileSize.x, selfTile.y + direction.y * tileSize.y);
        if (world.isDestructibleWallAt(adjacentTile, tileSize) && self.canPlaceBomb()) {
            return Decision{Vector2(-direction.x, -direction.y), true};
        }
    }

    // 4. Seek a nearby power-up.
    if (const std::optional<Vector2> powerUpPosition = world.findNearestPowerUpTile(self.getPosition());
        powerUpPosition.has_value()) {
        const float distanceTiles =
            std::sqrt(squaredDistance(self.getPosition(), *powerUpPosition)) / std::max(tileSize.x, tileSize.y);
        if (distanceTiles <= powerUpSearchRadiusTiles) {
            return Decision{towards(self.getPosition(), *powerUpPosition), false};
        }
    }

    // 5. Otherwise, work toward the nearest destructible wall, so there is eventually something
    //    adjacent to bomb (step 3 picks it up once reached).
    if (const std::optional<Vector2> wallPosition = world.findNearestDestructibleWallTile(self.getPosition());
        wallPosition.has_value()) {
        return Decision{towards(self.getPosition(), *wallPosition), false};
    }

    return Decision{Vector2(0.f, 0.f), false};
}

} // namespace core
