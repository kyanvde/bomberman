#ifndef BOMBERMAN_GAME_VIEWS_GRASSVIEW_H
#define BOMBERMAN_GAME_VIEWS_GRASSVIEW_H

#include "EntityView.h"

namespace game {

/**
 * @brief The GrassView class represents the visual representation of a grass entity in the game.
 * It inherits from EntityView and provides specific rendering for grass entities.
 */
class GrassView final : public EntityView {
public:
    /**
     * @brief Constructs a new GrassView object with the specified EntityModel and sprite frame.
     * @param model A reference to the EntityModel that this view represents.
     * @param sprite The sprite frame used for rendering the grass entity.
     */
    GrassView(const core::EntityModel& model, const core::SpriteFrame& sprite) : EntityView(model, sprite) {}
};

} // namespace game

#endif // BOMBERMAN_GAME_VIEWS_GRASSVIEW_H