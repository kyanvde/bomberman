#include "PowerUp.h"

namespace core {
PowerUp::PowerUp(const Vector2& pos, const Vector2& size, const PowerUpType type)
    : EntityModel(pos, size), type(type) {}
} // namespace core