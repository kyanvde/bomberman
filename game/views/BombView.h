#ifndef BOMBERMAN_GAME_VIEWS_BOMBVIEW_H
#define BOMBERMAN_GAME_VIEWS_BOMBVIEW_H

#include "EntityView.h"

class BombView : public EntityView {
public:
    BombView(const core::EntityModel& model, const core::SpriteFrame& sprite)
    : EntityView(model, sprite) {}
};

#endif // BOMBERMAN_GAME_VIEWS_BOMBVIEW_H