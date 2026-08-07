#ifndef BOMBERMAN_GAME_VIEWS_WALLVIEW_H
#define BOMBERMAN_GAME_VIEWS_WALLVIEW_H

#include "EntityView.h"

/**
 * @brief The WallView class represents the visual representation of a wall entity in the game.
 * It inherits from EntityView and provides specific rendering for wall entities.
 */
class WallView : public EntityView {
public:
    /**
	 * @brief Constructs a new WallView object with the specified EntityModel and sprite frame.
	 * @param model A reference to the EntityModel that this view represents.
	 * @param sprite The sprite frame used for rendering the wall entity.
	 */
	WallView(const core::EntityModel& model, const core::SpriteFrame& sprite)
	    : EntityView(model, sprite) {}
};

#endif // BOMBERMAN_GAME_VIEWS_WALLVIEW_H