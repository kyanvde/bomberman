#include "Explosion.h"

#include "World.h"

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
} // namespace core
