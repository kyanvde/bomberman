#ifndef BOMBERMAN_GAME_VIEWS_EXPLOSIONVIEW_H
#define BOMBERMAN_GAME_VIEWS_EXPLOSIONVIEW_H

#include "EntityView.h"

namespace game {

/**
 * @brief The ExplosionView class represents the visual representation of an explosion entity in
 * the game. It inherits from EntityView and grows the drawn sprite while fading it out over the
 * explosion's own lifetime, using whichever placeholder sprite frame the factory attached (a
 * dedicated explosion frame can be swapped in later without touching this animation logic).
 */
class ExplosionView : public EntityView {
public:
    /**
     * @brief Constructs a new ExplosionView object with the specified EntityModel and sprite frame.
     * @param model A reference to the EntityModel that this view represents.
     * @param sprite The sprite frame used for rendering the explosion entity.
     */
    ExplosionView(const core::EntityModel& model, const core::SpriteFrame& sprite);

    /**
     * @brief Re-syncs from the model as usual, then grows the drawn size and fades the sprite's
     * opacity based on the explosion's own lifetime fraction: small and opaque when just spawned,
     * largest and fully transparent right before it disappears.
     */
    void update(const core::GameEvent& event = {}) override;
};

} // namespace game

#endif // BOMBERMAN_GAME_VIEWS_EXPLOSIONVIEW_H
