#ifndef BOMBERMAN_GAME_VIEWS_WALLVIEW_H
#define BOMBERMAN_GAME_VIEWS_WALLVIEW_H

#include "EntityView.h"

class WallView : public EntityView {
public:
	WallView(const core::Vector2& position, const core::Vector2& size, const core::SpriteFrame& sprite)
	    : EntityView(position, size, sprite) {}
};

#endif // BOMBERMAN_GAME_VIEWS_WALLVIEW_H