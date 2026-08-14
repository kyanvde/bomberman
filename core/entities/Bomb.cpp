#include "Bomb.h"

namespace core {
Bomb::Bomb(const Vector2& pos, const Vector2& size) : EntityModel(pos, size) {
    setAnimationType(AnimationType::BombTicking);
    Subject::notify();
}

int Bomb::renderLayer() const noexcept { return 1; }

} // namespace core