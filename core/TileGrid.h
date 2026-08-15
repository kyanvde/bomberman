#ifndef BOMBERMAN_CORE_TILEGRID_H
#define BOMBERMAN_CORE_TILEGRID_H

#include "CharacterColor.h"

#include <cstddef>
#include <vector>

namespace core {

/**
 * @brief Integer coordinates of a single tile within the world grid, with (0, 0) at the top-left
 * corner. Lets navigation code reason in whole tiles instead of floating-point world positions,
 * which sidesteps the boundary-touching comparison problems those invite.
 */
struct GridCoord {
    int x = 0;
    int y = 0;
};

[[nodiscard]] inline bool operator==(const GridCoord& left, const GridCoord& right) noexcept {
    return left.x == right.x && left.y == right.y;
}

[[nodiscard]] inline bool operator!=(const GridCoord& left, const GridCoord& right) noexcept {
    return !(left == right);
}

/**
 * @brief Where a single living character stands, for navigation purposes.
 */
struct CharacterTile {
    GridCoord coord;
    CharacterColor color;
};

/**
 * @brief An immutable, tile-indexed snapshot of everything the AI needs to know about the world to
 * navigate it: which tiles are blocked, which hold something worth reaching, and which are inside
 * a live bomb's blast.
 *
 * It exists so an AI controller can run a breadth-first search over the map without querying the
 * World once per tile (which would be O(tiles * entities) per search); World::buildTileGrid fills
 * it in a single pass over the entities instead. Being a plain snapshot also means pathfinding can
 * never accidentally mutate the world it is reasoning about.
 */
struct TileGrid {
    int width = 0;
    int height = 0;

    /// Tiles holding a wall of either kind, which blocks both movement and explosions.
    std::vector<char> wall;

    /// Tiles holding a destructible wall specifically (a subset of wall).
    std::vector<char> destructible;

    /// Tiles holding a bomb, which blocks movement once armed.
    std::vector<char> bomb;

    /// Tiles holding a power-up waiting to be picked up.
    std::vector<char> powerUp;

    /// Tiles currently inside some live bomb's eventual blast.
    std::vector<char> dangerous;

    /// Where each living character stands.
    std::vector<CharacterTile> characters;

    /**
     * @brief Checks whether the given coordinate lies within the grid.
     */
    [[nodiscard]] bool inside(const GridCoord& coord) const noexcept;

    /**
     * @brief Converts a coordinate to its index into the per-tile vectors. Only meaningful for a
     * coordinate that is inside() the grid.
     */
    [[nodiscard]] std::size_t index(const GridCoord& coord) const noexcept;

    /**
     * @brief Checks whether a wall of either kind occupies the tile. Out-of-bounds counts as a
     * wall, so callers never have to bounds-check separately.
     */
    [[nodiscard]] bool isWall(const GridCoord& coord) const noexcept;

    /**
     * @brief Checks whether a destructible wall (one a bomb could clear) occupies the tile.
     */
    [[nodiscard]] bool isDestructible(const GridCoord& coord) const noexcept;

    /**
     * @brief Checks whether a bomb occupies the tile.
     */
    [[nodiscard]] bool hasBomb(const GridCoord& coord) const noexcept;

    /**
     * @brief Checks whether a power-up waits on the tile.
     */
    [[nodiscard]] bool hasPowerUp(const GridCoord& coord) const noexcept;

    /**
     * @brief Checks whether the tile is inside some live bomb's eventual blast.
     */
    [[nodiscard]] bool isDangerous(const GridCoord& coord) const noexcept;

    /**
     * @brief Checks whether a character could stand on the tile: inside the grid, and free of both
     * walls and bombs. Deliberately ignores danger, since fleeing a blast usually means crossing
     * part of it.
     */
    [[nodiscard]] bool isWalkable(const GridCoord& coord) const noexcept;

    /**
     * @brief Checks whether any destructible wall is left anywhere on the map.
     */
    [[nodiscard]] bool anyDestructibleRemaining() const noexcept;

    /**
     * @brief Checks whether a bomb placed at origin with the given radius would reach target,
     * accounting for walls stopping the blast. Used to decide whether a tile is a safe retreat
     * from a bomb the AI is considering placing but has not placed yet.
     * @param origin The tile the hypothetical bomb sits on.
     * @param radius The hypothetical bomb's blast radius, in tiles, in each direction.
     * @param target The tile to test.
     * @return True if the blast would reach target, false otherwise.
     */
    [[nodiscard]] bool blastReaches(const GridCoord& origin, int radius, const GridCoord& target) const noexcept;
};

} // namespace core

#endif // BOMBERMAN_CORE_TILEGRID_H
