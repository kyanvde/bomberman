#include "Wall.h"

namespace core {
    Wall::Wall(const Vector2& pos, const Vector2& size, bool destructible) : EntityModel(pos, size) {}
} // namespace core