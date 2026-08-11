#include "CharacterView.h"

CharacterView::CharacterView(const core::EntityModel& model, const core::SpriteFrame& sprite)
    : EntityView(model, sprite) {
    animations.insert({core::AnimationType::Idle, core::Animation({
        core::SpriteFrame(sprite.texturePath, core::Vector2(16.f, 24.f), core::Vector2(1.f, 1.f), sprite.margin, sprite.spacing)
    }, 0.5f)});

    animations.insert({core::AnimationType::WalkRight, core::Animation({
        core::SpriteFrame(sprite.texturePath, core::Vector2(16.f, 24.f), core::Vector2(0.f, 2.f), sprite.margin, sprite.spacing),
        core::SpriteFrame(sprite.texturePath, core::Vector2(16.f, 24.f), core::Vector2(1.f, 2.f), sprite.margin, sprite.spacing),
        core::SpriteFrame(sprite.texturePath, core::Vector2(16.f, 24.f), core::Vector2(2.f, 2.f), sprite.margin, sprite.spacing)
    }, 0.15f)});

    animations.insert({core::AnimationType::WalkLeft, core::Animation({
        core::SpriteFrame(sprite.texturePath, core::Vector2(16.f, 24.f), core::Vector2(0.f, 4.f), sprite.margin, sprite.spacing),
        core::SpriteFrame(sprite.texturePath, core::Vector2(16.f, 24.f), core::Vector2(1.f, 4.f), sprite.margin, sprite.spacing),
        core::SpriteFrame(sprite.texturePath, core::Vector2(16.f, 24.f), core::Vector2(2.f, 4.f), sprite.margin, sprite.spacing)
    }, 0.15f)});

    animations.insert({core::AnimationType::WalkUp, core::Animation({
        core::SpriteFrame(sprite.texturePath, core::Vector2(16.f, 24.f), core::Vector2(0.f, 3.f), sprite.margin, sprite.spacing),
        core::SpriteFrame(sprite.texturePath, core::Vector2(16.f, 24.f), core::Vector2(1.f, 3.f), sprite.margin, sprite.spacing),
        core::SpriteFrame(sprite.texturePath, core::Vector2(16.f, 24.f), core::Vector2(2.f, 3.f), sprite.margin, sprite.spacing)
    }, 0.15f)});

    animations.insert({core::AnimationType::WalkDown, core::Animation({
        core::SpriteFrame(sprite.texturePath, core::Vector2(16.f, 24.f), core::Vector2(0.f, 1.f), sprite.margin, sprite.spacing),
        core::SpriteFrame(sprite.texturePath, core::Vector2(16.f, 24.f), core::Vector2(1.f, 1.f), sprite.margin, sprite.spacing),
        core::SpriteFrame(sprite.texturePath, core::Vector2(16.f, 24.f), core::Vector2(2.f, 1.f), sprite.margin, sprite.spacing)
    }, 0.15f)});
}
