#ifndef BOMBERMAN_GAME_VIEWS_CHARACTERVIEW_H
#define BOMBERMAN_GAME_VIEWS_CHARACTERVIEW_H

#include "EntityView.h"

class CharacterView : public EntityView {
public:
    CharacterView(const core::EntityModel& model, const core::SpriteFrame& sprite)
    : EntityView(model, sprite) {}
};

#endif // BOMBERMAN_GAME_VIEWS_CHARACTERVIEW_H