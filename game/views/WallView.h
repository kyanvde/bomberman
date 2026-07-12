#ifndef BOMBERMAN_GAME_VIEWS_WALLVIEW_H
#define BOMBERMAN_GAME_VIEWS_WALLVIEW_H

#include "EntityView.h"

class WallView : public EntityView {
public:
	WallView(const core::EntityModel& model, const core::SpriteFrame& sprite)
	    : EntityView(model, sprite) {}
};

#endif // BOMBERMAN_GAME_VIEWS_WALLVIEW_H