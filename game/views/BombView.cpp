#include "BombView.h"

namespace game {

BombView::BombView(const core::EntityModel& model, const core::SpriteFrame& sprite) : EntityView(model, sprite) {
    animations.insert({core::AnimationType::BombTicking,
                       core::Animation(
                           {
                               core::SpriteFrame(sprite.texturePath, core::Vector2(16.f, 16.f),
                                                 core::Vector2(6.f, 0.f), sprite.margin, sprite.spacing),
                               core::SpriteFrame(sprite.texturePath, core::Vector2(16.f, 16.f),
                                                 core::Vector2(5.f, 0.f), sprite.margin, sprite.spacing),
                               core::SpriteFrame(sprite.texturePath, core::Vector2(16.f, 16.f),
                                                 core::Vector2(4.f, 0.f), sprite.margin, sprite.spacing),
                               core::SpriteFrame(sprite.texturePath, core::Vector2(16.f, 16.f),
                                                 core::Vector2(5.f, 0.f), sprite.margin, sprite.spacing),
                           },
                           0.15f)});
}

} // namespace game
