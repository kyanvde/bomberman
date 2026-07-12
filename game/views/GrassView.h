#ifndef BOMBERMAN_GAME_VIEWS_GRASSVIEW_H
#define BOMBERMAN_GAME_VIEWS_GRASSVIEW_H

#include "EntityView.h"

class GrassView : public EntityView {
public:
    GrassView(const core::EntityModel& model, const core::SpriteFrame& sprite)
        : EntityView(model, sprite) {}
};

#endif // BOMBERMAN_GAME_VIEWS_GRASSVIEW_H