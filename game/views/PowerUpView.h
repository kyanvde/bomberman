
#ifndef BOMBERMAN_GAME_VIEWS_POWERUPVIEW_H
#define BOMBERMAN_GAME_VIEWS_POWERUPVIEW_H

#include "EntityView.h"

namespace game {

/**
 * @brief The PowerUpView class represents the visual representation of a power-up entity in the game.
 * It inherits from EntityView and provides specific rendering for power-up entities.
 */
class PowerUpView : public EntityView {
public:
    /**
     * @brief Constructs a new PowerUpView object with the specified EntityModel and sprite frame.
     * @param model A reference to the EntityModel that this view represents.
     * @param sprite The sprite frame used for rendering the power-up entity.
     */
    PowerUpView(const core::EntityModel& model, const core::SpriteFrame& sprite) : EntityView(model, sprite) {}
};

} // namespace game

#endif // BOMBERMAN_GAME_VIEWS_POWERUPVIEW_H