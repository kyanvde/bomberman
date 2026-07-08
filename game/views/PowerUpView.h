
#ifndef BOMBERMAN_GAME_VIEWS_POWERUPVIEW_H
#define BOMBERMAN_GAME_VIEWS_POWERUPVIEW_H

#include "EntityView.h"

class PowerUpView : public EntityView {
public:
    PowerUpView(const core::Vector2& position, const core::Vector2& size, const core::SpriteFrame& sprite)
    : EntityView(position, size, sprite) {}
};

#endif // BOMBERMAN_GAME_VIEWS_POWERUPVIEW_H