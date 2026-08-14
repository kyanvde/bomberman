#ifndef BOMBERMAN_CORE_COLLISION_H
#define BOMBERMAN_CORE_COLLISION_H

#include "Vector2.h"

namespace core {

/**
 * @brief Checks whether two axis-aligned rectangles, given as a position (top-left corner) and
 * size, overlap. Uses plain arithmetic only, per the assignment's requirement that collision
 * detection in the logic library not rely on SFML utilities.
 * @param aPos The top-left corner of the first rectangle.
 * @param aSize The size of the first rectangle.
 * @param bPos The top-left corner of the second rectangle.
 * @param bSize The size of the second rectangle.
 * @return True if the two rectangles overlap, false otherwise.
 */
[[nodiscard]] inline bool intersects(const Vector2& aPos, const Vector2& aSize, const Vector2& bPos,
                                     const Vector2& bSize) noexcept {
    const bool overlapX = aPos.x < bPos.x + bSize.x && aPos.x + aSize.x > bPos.x;
    const bool overlapY = aPos.y < bPos.y + bSize.y && aPos.y + aSize.y > bPos.y;
    return overlapX && overlapY;
}

} // namespace core

#endif // BOMBERMAN_CORE_COLLISION_H
