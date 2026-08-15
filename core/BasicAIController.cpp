#include "BasicAIController.h"

#include "CharacterColor.h"
#include "EntityModel.h"
#include "TileGrid.h"
#include "World.h"

#include <algorithm>
#include <cstddef>
#include <cstdlib>
#include <limits>
#include <optional>
#include <queue>
#include <vector>

namespace core {
namespace {

constexpr GridCoord cardinalSteps[] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};

// How far a bot already caught in a blast will look for somewhere safe to stand. Running a long
// way is still better than giving up, so this is generous.
constexpr int fleeSearchDepth = 12;

// How far a bot will accept having to run when deciding whether to place a bomb. This has to stay
// within what it can actually cover before the fuse burns down -- a route it cannot finish in time
// is not an escape at all -- so it is deliberately shorter than fleeSearchDepth.
constexpr int bombEscapeSearchDepth = 4;

Vector2 toDirection(const GridCoord& from, const GridCoord& to) {
    return Vector2(static_cast<float>(to.x - from.x), static_cast<float>(to.y - from.y));
}

/**
 * @brief The outcome of a successful search: which way to step first, and how far the goal is.
 */
struct PathResult {
    Vector2 firstStep;
    int distance = 0;
};

/**
 * @brief Searches the tile grid for the best tile satisfying isGoal, giving back the first step of
 * the route to it.
 *
 * Searching outward like this is what lets the bots actually get anywhere: walking greedily in a
 * target's direction gets stuck against the first pillar in the way, whereas this routes around
 * obstacles. Routes are ranked by exposure to blasts before length (see dangerPenalty below), so
 * the shortest way is not taken when a safer way exists.
 *
 * A bot already standing where it wants to be is told to hold position rather than sent to the
 * next-best tile. Without that, a bot that has arrived next to a wall but cannot bomb it yet (its
 * one bomb still ticking elsewhere) would be routed to some other wall, arrive, be routed back,
 * and end up vibrating on the spot forever.
 *
 * @param grid The navigation snapshot to search.
 * @param start The tile to search outward from.
 * @param isGoal Predicate deciding whether a tile is worth reaching.
 * @param isPassable Predicate deciding whether a tile may be routed through.
 * @param maxDistance The furthest, in tiles, the search may travel.
 * @return The first step and distance, or std::nullopt if no goal is reachable within maxDistance.
 */
template <typename GoalFn, typename PassableFn>
std::optional<PathResult> findPath(const TileGrid& grid, const GridCoord& start, GoalFn isGoal, PassableFn isPassable,
                                   const int maxDistance) {
    if (!grid.inside(start) || maxDistance <= 0) {
        return std::nullopt;
    }

    if (isGoal(start)) {
        return PathResult{Vector2(0.f, 0.f), 0};
    }

    // Routes are ranked by how many threatened tiles they cross first and how long they are second,
    // both packed into one number so a single search minimises them in that order. Treating the two
    // as interchangeable is how a bot ends up diving through a blast that is seconds from going off
    // to reach a refuge a tile nearer than the safe way round.
    constexpr int dangerPenalty = 1000;

    // Each queued tile remembers the very first step taken toward it, so the search can return a
    // direction to walk without having to reconstruct the whole path afterwards.
    struct Entry {
        GridCoord coord;
        GridCoord firstStep;
        int length = 0;
        int cost = 0;
    };

    struct CheapestFirst {
        bool operator()(const Entry& left, const Entry& right) const { return left.cost > right.cost; }
    };

    const std::size_t cells = static_cast<std::size_t>(grid.width) * static_cast<std::size_t>(grid.height);
    std::vector<int> bestCost(cells, std::numeric_limits<int>::max());
    std::priority_queue<Entry, std::vector<Entry>, CheapestFirst> pending;

    const auto relax = [&](const GridCoord& from, const GridCoord* firstStep, const int length, const int cost) {
        for (const GridCoord& step : cardinalSteps) {
            const GridCoord neighbour{from.x + step.x, from.y + step.y};
            if (!grid.inside(neighbour) || !isPassable(neighbour)) {
                continue;
            }

            const int nextLength = length + 1;
            if (nextLength > maxDistance) {
                continue;
            }

            const int nextCost = cost + 1 + (grid.isDangerous(neighbour) ? dangerPenalty : 0);
            const std::size_t cell = grid.index(neighbour);
            if (nextCost >= bestCost[cell]) {
                continue;
            }

            bestCost[cell] = nextCost;
            pending.push(Entry{neighbour, firstStep != nullptr ? *firstStep : neighbour, nextLength, nextCost});
        }
    };

    relax(start, nullptr, 0, 0);

    while (!pending.empty()) {
        const Entry current = pending.top();
        pending.pop();

        if (current.cost > bestCost[grid.index(current.coord)]) {
            continue; // superseded by a cheaper route found since this was queued
        }

        if (isGoal(current.coord)) {
            return PathResult{toDirection(start, current.firstStep), current.length};
        }

        relax(current.coord, &current.firstStep, current.length, current.cost);
    }

    return std::nullopt;
}

bool isDestructibleAdjacent(const TileGrid& grid, const GridCoord& coord) {
    for (const GridCoord& step : cardinalSteps) {
        if (grid.isDestructible(GridCoord{coord.x + step.x, coord.y + step.y})) {
            return true;
        }
    }
    return false;
}

bool isEnemyWithinOneTile(const TileGrid& grid, const GridCoord& coord, const CharacterColor& self) {
    for (const CharacterTile& character : grid.characters) {
        if (character.color == self) {
            continue;
        }
        if (std::abs(character.coord.x - coord.x) + std::abs(character.coord.y - coord.y) <= 1) {
            return true;
        }
    }
    return false;
}

/**
 * @brief Checks whether a character could come to a stop on this tile and genuinely be out of
 * harm's way.
 *
 * Being clear is not enough on its own. A character is exactly one tile in size and stops wherever
 * its last step left it, so at rest it generally still overlaps one neighbouring tile -- and an
 * explosion catches anything it overlaps. A tile with a blast on either side therefore offers no
 * resting place at all: the character ends up edging back and forth between the two until the
 * bombs go off. Requiring the neighbours to be clear too (or solid, in which case there is nothing
 * to overlap into) gives somewhere it can actually stand still and survive.
 *
 * @param grid The navigation snapshot to consult.
 * @param coord The tile to judge.
 * @param isTileDangerous Decides whether a tile is threatened; taking this as a parameter lets the
 * same rule cover both live bombs and a bomb the AI is only considering placing.
 * @return True if a character could stop here and be safe.
 */
template <typename DangerFn>
bool canRestAt(const TileGrid& grid, const GridCoord& coord, DangerFn isTileDangerous) {
    if (isTileDangerous(coord)) {
        return false;
    }

    // Along each axis a character at rest overlaps at most one of the two neighbours -- whichever
    // side its last step left it leaning toward. A solid neighbour is therefore welcome rather than
    // a problem: the character can press flush against it and overlap nothing at all on that axis.
    // Only when both neighbours are open does it need both of them to be survivable, because then
    // it cannot control which one it ends up leaning into.
    const auto axisIsSafe = [&](const GridCoord& negative, const GridCoord& positive) {
        if (grid.isWall(negative) || grid.isWall(positive)) {
            return true;
        }
        return !isTileDangerous(negative) && !isTileDangerous(positive);
    };

    return axisIsSafe(GridCoord{coord.x - 1, coord.y}, GridCoord{coord.x + 1, coord.y}) &&
           axisIsSafe(GridCoord{coord.x, coord.y - 1}, GridCoord{coord.x, coord.y + 1});
}

/**
 * @brief Works out which way to run from a bomb sitting at bombCoord, before deciding to place it.
 *
 * This is what stops a bot blowing itself up. A bomb covers its own tile plus everything within
 * its radius, so with even a radius of 1 every immediately adjacent tile is inside the blast --
 * safety is only ever found by walking through the danger and out the far side, or around a
 * corner. Anything that only inspects neighbouring tiles concludes there is nowhere to go and
 * stands still on the bomb.
 *
 * @param grid The navigation snapshot to search.
 * @param start The tile the bot is standing on.
 * @param bombCoord The tile the hypothetical bomb would sit on.
 * @param radius The hypothetical bomb's blast radius.
 * @return The direction to retreat in, or std::nullopt if there is no way out (so: do not bomb).
 */
std::optional<Vector2> escapeStep(const TileGrid& grid, const GridCoord& start, const GridCoord& bombCoord,
                                  const int radius) {
    const auto isThreatened = [&](const GridCoord& coord) {
        return grid.isDangerous(coord) || grid.blastReaches(bombCoord, radius, coord);
    };
    const auto isSafe = [&](const GridCoord& coord) { return canRestAt(grid, coord, isThreatened); };
    // The way out may pass through what the new bomb is about to cover -- that is unavoidable, it
    // is standing on it -- but not through a blast that is already live and counting down. Ignoring
    // that, a bot happily drops a bomb and retreats along a corridor another of its bombs is about
    // to fill, and is caught halfway by the older one.
    const auto isPassable = [&](const GridCoord& coord) { return grid.isWalkable(coord) && !grid.isDangerous(coord); };

    if (const std::optional<PathResult> path = findPath(grid, start, isSafe, isPassable, bombEscapeSearchDepth)) {
        return path->firstStep;
    }
    return std::nullopt;
}

} // namespace

BasicAIController::BasicAIController(const float aggressionRadiusTiles, const float powerUpSearchRadiusTiles)
    : aggressionRadiusTiles(aggressionRadiusTiles), powerUpSearchRadiusTiles(powerUpSearchRadiusTiles) {}

Decision BasicAIController::decide(const World& world, const EntityModel& self) const {
    const TileGrid grid = world.buildTileGrid();
    const GridCoord selfCoord = world.toGrid(self.getPosition(), self.getSize());
    if (!grid.inside(selfCoord)) {
        return Decision{};
    }

    const CharacterColor color = self.getCharacterColor().value_or(CharacterColor::White);
    const int radius = self.getBombRadius();

    // Routing for everything except fleeing. It sticks to tiles the character could safely come to
    // a stop on, not merely ones that happen to be clear: walking to the very edge of a blast
    // leaves the body hanging into it, which counts as being in danger again, and the bot ends up
    // torn between advancing and retreating on alternate ticks.
    const auto isSafePassable = [&](const GridCoord& coord) {
        return grid.isWalkable(coord) &&
               canRestAt(grid, coord, [&](const GridCoord& tile) { return grid.isDangerous(tile); });
    };

    // Fleeing has to allow crossing threatened tiles, since that is usually the only way out.
    const auto isPassable = [&](const GridCoord& coord) { return grid.isWalkable(coord); };

    // 1. Get out of a blast, above all else. Judged on every tile the character's body overlaps,
    //    not just the one it is centred on: an explosion catches anything overlapping its tile, so
    //    a bot that has only just crossed into a safe tile is still half inside the blast and has
    //    to keep going.
    const std::vector<GridCoord> footprint = world.tilesOverlapping(self.getPosition(), self.getSize());
    const bool inDanger = std::any_of(footprint.begin(), footprint.end(),
                                      [&](const GridCoord& coord) { return grid.isDangerous(coord); });

    const auto isTileDangerous = [&](const GridCoord& tile) { return grid.isDangerous(tile); };

    if (inDanger) {
        // Already on a tile it can safely settle on, with only the trailing part of the body still
        // hanging into a blast: finish easing into this tile instead of setting off for another
        // one. Whatever makes this tile safe to stop on is a wall on the far side, so the nudge
        // ends flush against it rather than drifting back and forth.
        if (canRestAt(grid, selfCoord, isTileDangerous)) {
            for (const GridCoord& coord : footprint) {
                if (coord != selfCoord && grid.isDangerous(coord)) {
                    return Decision{toDirection(coord, selfCoord), false};
                }
            }
        }

        // Head for the nearest tile the character can actually come to a stop on. Aiming merely
        // for a clear tile is what leaves a bot stranded on a one-tile gap with blasts on both
        // sides: nowhere on it is genuinely out of reach, so it shuffles between the two edges
        // until the bombs go off.
        const auto isRestingPlace = [&](const GridCoord& coord) {
            return coord != selfCoord && canRestAt(grid, coord, isTileDangerous);
        };
        if (const std::optional<PathResult> path =
                findPath(grid, selfCoord, isRestingPlace, isPassable, fleeSearchDepth)) {
            return Decision{path->firstStep, false};
        }

        // Nowhere properly safe within reach, so settle for anything not currently in a blast.
        const auto isClear = [&](const GridCoord& coord) { return coord != selfCoord && !grid.isDangerous(coord); };
        if (const std::optional<PathResult> path = findPath(grid, selfCoord, isClear, isPassable, fleeSearchDepth)) {
            return Decision{path->firstStep, false};
        }

        return Decision{Vector2(0.f, 0.f), false}; // boxed in; nothing better to do than sit tight
    }

    // 2. Bomb an enemy standing right next to us -- but only if we have somewhere to run.
    if (self.canPlaceBomb() && isEnemyWithinOneTile(grid, selfCoord, color)) {
        if (const std::optional<Vector2> retreat = escapeStep(grid, selfCoord, selfCoord, radius)) {
            return Decision{*retreat, true};
        }
    }

    // 3. Bomb an adjacent destructible wall, again only with an escape route in hand.
    if (self.canPlaceBomb() && isDestructibleAdjacent(grid, selfCoord)) {
        if (const std::optional<Vector2> retreat = escapeStep(grid, selfCoord, selfCoord, radius)) {
            return Decision{*retreat, true};
        }
    }

    // 4. Pick up a power-up that is close enough to be worth the detour.
    const auto isPowerUpTile = [&](const GridCoord& coord) { return grid.hasPowerUp(coord); };
    if (const std::optional<PathResult> path =
            findPath(grid, selfCoord, isPowerUpTile, isSafePassable, static_cast<int>(powerUpSearchRadiusTiles))) {
        return Decision{path->firstStep, false};
    }

    // 5. Hunt an enemy: only nearby ones while there are still walls to break, but anywhere on the
    //    map once breaking walls is no longer an option.
    const auto isNextToEnemy = [&](const GridCoord& coord) { return isEnemyWithinOneTile(grid, coord, color); };
    const int enemyRange =
        grid.anyDestructibleRemaining() ? static_cast<int>(aggressionRadiusTiles) : grid.width + grid.height;
    if (const std::optional<PathResult> path = findPath(grid, selfCoord, isNextToEnemy, isSafePassable, enemyRange)) {
        return Decision{path->firstStep, false};
    }

    // 6. Otherwise head for the nearest destructible wall, so that step 3 has something to do once
    //    we arrive.
    const auto isNextToWall = [&](const GridCoord& coord) { return isDestructibleAdjacent(grid, coord); };
    if (const std::optional<PathResult> path =
            findPath(grid, selfCoord, isNextToWall, isSafePassable, grid.width + grid.height)) {
        return Decision{path->firstStep, false};
    }

    return Decision{Vector2(0.f, 0.f), false};
}

} // namespace core
