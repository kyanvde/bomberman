#ifndef BOMBERMAN_GAME_VIEWS_CHARACTERVIEW_H
#define BOMBERMAN_GAME_VIEWS_CHARACTERVIEW_H

#include "Animation.h"
#include "EntityView.h"

/**
 * @brief The CharacterView class represents the visual representation of a character entity in the game.
 * It inherits from EntityView and provides specific rendering for character entities.
 */
class CharacterView : public EntityView {
    /**
     * @brief A key-value store of all the available character animations.
     */
    std::unordered_map<std::string, core::Animation> animations {
        {"idle", core::Animation({core::SpriteFrame(sprite.texturePath, core::Vector2(16.f, 24.f), core::Vector2(1.f, 1.f), sprite.margin, sprite.spacing)}, 0.5f)},
        {"walk", core::Animation({
            core::SpriteFrame(sprite.texturePath, core::Vector2(16.f, 24.f), core::Vector2(0.f, 1.f), sprite.margin, sprite.spacing),
            core::SpriteFrame(sprite.texturePath, core::Vector2(16.f, 24.f), core::Vector2(1.f, 1.f), sprite.margin, sprite.spacing),
            core::SpriteFrame(sprite.texturePath, core::Vector2(16.f, 24.f), core::Vector2(2.f, 1.f), sprite.margin, sprite.spacing)
        }, 0.15f)}
    };

    /**
     * @brief The key of the current active animation.
     */
    std::string currentAnimation;

public:
    /**
     * @brief Constructs a new CharacterView object with the specified EntityModel and sprite frame.
     * @param model A reference to the EntityModel that this view represents.
     * @param sprite The sprite frame used for rendering the character entity.
     */
    CharacterView(const core::EntityModel& model, const core::SpriteFrame& sprite)
    : EntityView(model, sprite) {}

    /**
     * Updates the character logic.
     */
    void update() override;

    /**
     * Activates an animation.
     * @param animationKey The key of the animation to activate.
     */
    void setAnimation(const std::string& animationKey);
};

#endif // BOMBERMAN_GAME_VIEWS_CHARACTERVIEW_H