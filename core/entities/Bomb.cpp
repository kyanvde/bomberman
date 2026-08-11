#include "Bomb.h"

namespace core {
    Bomb::Bomb(const Vector2& pos, const Vector2& size) : EntityModel(pos, size) {}

    int Bomb::renderLayer() const noexcept {
        return 1;
    }
} // namespace core