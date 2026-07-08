#ifndef BOMBERMAN_GAME_VIEWS_GRASSVIEW_H
#define BOMBERMAN_GAME_VIEWS_GRASSVIEW_H

#include "EntityView.h"

class GrassView : public EntityView {
public:
    GrassView(const core::Vector2& position, const core::Vector2& size, const core::SpriteFrame& sprite)
        : EntityView(position, size, sprite) {}
};

#endif // BOMBERMAN_GAME_VIEWS_GRASSVIEW_H