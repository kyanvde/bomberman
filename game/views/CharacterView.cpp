#include "CharacterView.h"

#include "CharacterColor.h"

CharacterView::CharacterView(const core::EntityModel& model, const core::SpriteFrame& sprite, const core::CharacterColor& color)
    : EntityView(model, sprite), color(color) {

    core::Vector2 basePosition;

    switch (color) {
        case core::CharacterColor::White:
            basePosition = core::Vector2(1.f, 1.f);
            break;
        case core::CharacterColor::Blue:
            basePosition = core::Vector2(4.f, 1.f);
            break;
        case core::CharacterColor::Red:
            basePosition = core::Vector2(7.f, 1.f);
            break;
        case core::CharacterColor::Black:
            basePosition = core::Vector2(10.f, 1.f);
            break;
    }

    animations.insert({core::AnimationType::Idle, core::Animation({
        core::SpriteFrame(sprite.texturePath, core::Vector2(16.f, 24.f), basePosition, sprite.margin, sprite.spacing)
    }, 0.5f)});

    animations.insert({core::AnimationType::WalkDown, core::Animation({
        core::SpriteFrame(sprite.texturePath, core::Vector2(16.f, 24.f), basePosition - core::Vector2(1.f, 0.f), sprite.margin, sprite.spacing),
        core::SpriteFrame(sprite.texturePath, core::Vector2(16.f, 24.f), basePosition, sprite.margin, sprite.spacing),
        core::SpriteFrame(sprite.texturePath, core::Vector2(16.f, 24.f), basePosition + core::Vector2(1.f, 0.f), sprite.margin, sprite.spacing)
    }, 0.3f)});

    animations.insert({core::AnimationType::WalkRight, core::Animation({
        core::SpriteFrame(sprite.texturePath, core::Vector2(16.f, 24.f), basePosition + core::Vector2(-1.f, 1.f), sprite.margin, sprite.spacing),
        core::SpriteFrame(sprite.texturePath, core::Vector2(16.f, 24.f), basePosition + core::Vector2(0.f, 1.f), sprite.margin, sprite.spacing),
        core::SpriteFrame(sprite.texturePath, core::Vector2(16.f, 24.f), basePosition + core::Vector2(1.f, 1.f), sprite.margin, sprite.spacing)
    }, 0.3f)});

    animations.insert({core::AnimationType::WalkLeft, core::Animation({
        core::SpriteFrame(sprite.texturePath, core::Vector2(16.f, 24.f), basePosition + core::Vector2(-1.f, 3.f), sprite.margin, sprite.spacing),
        core::SpriteFrame(sprite.texturePath, core::Vector2(16.f, 24.f), basePosition + core::Vector2(0.f, 3.f), sprite.margin, sprite.spacing),
        core::SpriteFrame(sprite.texturePath, core::Vector2(16.f, 24.f), basePosition + core::Vector2(1.f, 3.f), sprite.margin, sprite.spacing)
    }, 0.3f)});

    animations.insert({core::AnimationType::WalkUp, core::Animation({
        core::SpriteFrame(sprite.texturePath, core::Vector2(16.f, 24.f), basePosition + core::Vector2(-1.f, 2.f), sprite.margin, sprite.spacing),
        core::SpriteFrame(sprite.texturePath, core::Vector2(16.f, 24.f), basePosition + core::Vector2(0.f, 2.f), sprite.margin, sprite.spacing),
        core::SpriteFrame(sprite.texturePath, core::Vector2(16.f, 24.f), basePosition + core::Vector2(1.f, 2.f), sprite.margin, sprite.spacing)
    }, 0.3f)});
}
