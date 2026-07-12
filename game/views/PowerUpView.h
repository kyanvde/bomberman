
#ifndef BOMBERMAN_GAME_VIEWS_POWERUPVIEW_H
#define BOMBERMAN_GAME_VIEWS_POWERUPVIEW_H

#include "EntityView.h"

class PowerUpView : public EntityView {
public:
    PowerUpView(const core::EntityModel& model, const core::SpriteFrame& sprite)
    : EntityView(model, sprite) {}
};

#endif // BOMBERMAN_GAME_VIEWS_POWERUPVIEW_H