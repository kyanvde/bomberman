#ifndef BOMBERMAN_GAME_VIEWS_EXPLOSIONVIEW_H
#define BOMBERMAN_GAME_VIEWS_EXPLOSIONVIEW_H

#include "EntityView.h"

namespace game {

/**
 * @brief The ExplosionView class represents the visual representation of an explosion entity in
 * the game. It inherits from EntityView and provides specific rendering for explosion entities.
 * Renders as a single static sprite for now; the grow-and-fade animation treatment is added in a
 * later step, once dedicated explosion sprite frames have been picked from the spritesheet.
 */
class ExplosionView : public EntityView {
public:
    /**
     * @brief Constructs a new ExplosionView object with the specified EntityModel and sprite frame.
     * @param model A reference to the EntityModel that this view represents.
     * @param sprite The sprite frame used for rendering the explosion entity.
     */
    ExplosionView(const core::EntityModel& model, const core::SpriteFrame& sprite) : EntityView(model, sprite) {}
};

} // namespace game

#endif // BOMBERMAN_GAME_VIEWS_EXPLOSIONVIEW_H
