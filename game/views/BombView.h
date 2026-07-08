#ifndef BOMBERMAN_GAME_VIEWS_BOMBVIEW_H
#define BOMBERMAN_GAME_VIEWS_BOMBVIEW_H

#include "EntityView.h"

class BombView : public EntityView {
public:
    BombView(const core::Vector2& position, const core::Vector2& size, const core::SpriteFrame& sprite)
    : EntityView(position, size, sprite) {}
};

#endif // BOMBERMAN_GAME_VIEWS_BOMBVIEW_H