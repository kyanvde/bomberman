#include "Explosion.h"

#include "World.h"

#include <algorithm>

namespace core {
namespace {
constexpr float explosionLifetimeSeconds = 0.4f;
} // namespace

Explosion::Explosion(const Vector2& pos, const Vector2& size)
    : EntityModel(pos, size), remainingLifetime(explosionLifetimeSeconds) {
    setAnimationType(AnimationType::Explosion);
    Subject::notify();
}

int Explosion::renderLayer() const noexcept { return 3; }

void Explosion::onTick(World& world, const EntityId selfId, const float deltaTime) {
    remainingLifetime -= deltaTime;
    if (remainingLifetime <= 0.f) {
        world.markForRemoval(selfId);
    }
}

float Explosion::getLifetimeFraction() const noexcept {
    const float elapsed = explosionLifetimeSeconds - remainingLifetime;
    return std::clamp(elapsed / explosionLifetimeSeconds, 0.f, 1.f);
}
} // namespace core
