#ifndef BOMBERMAN_GAME_VIEWS_CHARACTERVIEW_H
#define BOMBERMAN_GAME_VIEWS_CHARACTERVIEW_H

#include "EntityView.h"

class CharacterView : public EntityView {
public:
    CharacterView(const core::Vector2& position, const core::Vector2& size, const core::SpriteFrame& sprite)
    : EntityView(position, size, sprite) {}
};

#endif // BOMBERMAN_GAME_VIEWS_CHARACTERVIEW_H