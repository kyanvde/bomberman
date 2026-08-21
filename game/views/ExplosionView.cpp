#include "ExplosionView.h"

namespace game {

ExplosionView::ExplosionView(const core::EntityModel& model, const core::SpriteFrame& sprite)
    : EntityView(model, sprite) {}

void ExplosionView::update(const core::GameEvent& event) {
    EntityView::update(event);

    const float fraction = model.get().getLifetimeFraction();

    constexpr float minScale = 0.6f;
    constexpr float maxScale = 1.3f;
    const float scaleFactor = minScale + (maxScale - minScale) * fraction;

    const core::Vector2 grownSize(size.x * scaleFactor, size.y * scaleFactor);
    // Grow outward from the tile's center rather than its top-left corner.
    position = core::Vector2(position.x - (grownSize.x - size.x) / 2.f, position.y - (grownSize.y - size.y) / 2.f);
    size = grownSize;

    sprite.alpha = 1.f - fraction;
}

} // namespace game
