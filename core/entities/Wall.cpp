#include "Wall.h"

#include "Collision.h"

namespace core {
Wall::Wall(const Vector2& pos, const Vector2& size, const bool destructible)
    : EntityModel(pos, size), destructible(destructible) {}

bool Wall::blocksCharacterMovement(const Character&, const Vector2& characterPosition,
                                   const Vector2& characterSize) const {
    return intersects(characterPosition, characterSize, getPosition(), getSize());
}
} // namespace core