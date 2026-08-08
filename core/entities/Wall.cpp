#include "Wall.h"

namespace core {
    namespace {
        bool intersects(const Vector2& aPos, const Vector2& aSize, const Vector2& bPos, const Vector2& bSize) {
            const bool overlapX = aPos.x < bPos.x + bSize.x && aPos.x + aSize.x > bPos.x;
            const bool overlapY = aPos.y < bPos.y + bSize.y && aPos.y + aSize.y > bPos.y;
            return overlapX && overlapY;
        }
    }

    Wall::Wall(const Vector2& pos, const Vector2& size, [[maybe_unused]] bool destructible) : EntityModel(pos, size) {}

    bool Wall::blocksCharacterMovement(const Character&, const Vector2& characterPosition, const Vector2& characterSize) const {
        return intersects(characterPosition, characterSize, getPosition(), getSize());
    }
} // namespace core