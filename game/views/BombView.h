#ifndef BOMBERMAN_GAME_VIEWS_BOMBVIEW_H
#define BOMBERMAN_GAME_VIEWS_BOMBVIEW_H

#include "EntityView.h"

namespace game {

/**
 * @brief The BombView class represents the visual representation of a bomb entity in the game.
 * It inherits from EntityView and provides specific rendering for bomb entities.
 */
class BombView : public EntityView {
public:
    /**
     * @brief Constructs a new BombView object with the specified EntityModel and sprite frame.
     * @param model A reference to the EntityModel that this view represents.
     * @param sprite The sprite frame used for rendering the bomb entity.
     */
    BombView(const core::EntityModel& model, const core::SpriteFrame& sprite) : EntityView(model, sprite) {
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
};

} // namespace game

#endif // BOMBERMAN_GAME_VIEWS_BOMBVIEW_H