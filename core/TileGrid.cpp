#include "TileGrid.h"

#include <algorithm>
#include <cstdlib>

namespace core {

bool operator==(const GridCoord& left, const GridCoord& right) noexcept {
    return left.x == right.x && left.y == right.y;
}

bool operator!=(const GridCoord& left, const GridCoord& right) noexcept { return !(left == right); }

bool TileGrid::inside(const GridCoord& coord) const noexcept {
    return coord.x >= 0 && coord.y >= 0 && coord.x < width && coord.y < height;
}

std::size_t TileGrid::index(const GridCoord& coord) const noexcept {
    return static_cast<std::size_t>(coord.y) * static_cast<std::size_t>(width) + static_cast<std::size_t>(coord.x);
}

bool TileGrid::isWall(const GridCoord& coord) const noexcept {
    // Treating everything beyond the grid as solid means navigation code never has to bounds-check
    // separately -- walking off the map is blocked by the same rule that blocks walking into a wall.
    return !inside(coord) || wall[index(coord)] != 0;
}

bool TileGrid::isDestructible(const GridCoord& coord) const noexcept {
    return inside(coord) && destructible[index(coord)] != 0;
}

bool TileGrid::hasBomb(const GridCoord& coord) const noexcept { return inside(coord) && bomb[index(coord)] != 0; }

bool TileGrid::hasPowerUp(const GridCoord& coord) const noexcept { return inside(coord) && powerUp[index(coord)] != 0; }

bool TileGrid::isDangerous(const GridCoord& coord) const noexcept {
    return inside(coord) && dangerous[index(coord)] != 0;
}

bool TileGrid::isWalkable(const GridCoord& coord) const noexcept {
    return inside(coord) && wall[index(coord)] == 0 && bomb[index(coord)] == 0;
}

bool TileGrid::anyDestructibleRemaining() const noexcept {
    return std::any_of(destructible.begin(), destructible.end(), [](const char cell) { return cell != 0; });
}

bool TileGrid::blastReaches(const GridCoord& origin, const int radius, const GridCoord& target) const noexcept {
    if (!inside(origin) || !inside(target)) {
        return false;
    }
    if (origin == target) {
        return true; // the bomb's own tile always goes up
    }
    if (origin.x != target.x && origin.y != target.y) {
        return false; // a blast only travels along its own row and column
    }

    const int distance = std::abs(target.x - origin.x) + std::abs(target.y - origin.y);
    if (distance > radius) {
        return false;
    }

    const int stepX = target.x == origin.x ? 0 : (target.x > origin.x ? 1 : -1);
    const int stepY = target.y == origin.y ? 0 : (target.y > origin.y ? 1 : -1);

    for (int step = 1; step <= distance; ++step) {
        const GridCoord along{origin.x + stepX * step, origin.y + stepY * step};
        if (along == target) {
            return true;
        }
        if (isWall(along)) {
            return false; // the blast stops at the first wall, so it never reaches past it
        }
    }

    return true;
}

} // namespace core
