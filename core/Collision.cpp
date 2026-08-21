#include "Collision.h"

namespace core {

bool intersects(const Vector2& aPos, const Vector2& aSize, const Vector2& bPos, const Vector2& bSize) noexcept {
    const bool overlapX = aPos.x < bPos.x + bSize.x && aPos.x + aSize.x > bPos.x;
    const bool overlapY = aPos.y < bPos.y + bSize.y && aPos.y + aSize.y > bPos.y;
    return overlapX && overlapY;
}

} // namespace core
