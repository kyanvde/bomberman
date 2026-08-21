#include "PowerUpView.h"

namespace game {

PowerUpView::PowerUpView(const core::EntityModel &model, const core::SpriteFrame &sprite) : EntityView(
    model, sprite) {

    const core::Vector2 basePosition = sprite.cell;

    animations.insert(
        {core::AnimationType::Idle,
            core::Animation({
                core::SpriteFrame(sprite.texturePath, core::Vector2(16.f, 16.f), basePosition, sprite.margin, sprite.spacing),
                core::SpriteFrame(sprite.texturePath, core::Vector2(16.f, 16.f), basePosition + core::Vector2(0.f, 1.f), sprite.margin, sprite.spacing)
            },0.1f)});
}

} // namespace game
