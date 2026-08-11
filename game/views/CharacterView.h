#ifndef BOMBERMAN_GAME_VIEWS_CHARACTERVIEW_H
#define BOMBERMAN_GAME_VIEWS_CHARACTERVIEW_H

#include "Animation.h"
#include "EntityView.h"

namespace core {
    enum class CharacterColor;
}

/**
 * @brief The CharacterView class represents the visual representation of a character entity in the game.
 * It inherits from EntityView and provides specific rendering for character entities.
 */
class CharacterView : public EntityView {
    core::CharacterColor color;
public:
    /**
     * @brief Constructs a new CharacterView object with the specified EntityModel and sprite frame.
     * @param model A reference to the EntityModel that this view represents.
     * @param sprite The sprite frame used for rendering the character entity.
     * @param color The color of the character, which can be used to customize the appearance of the character sprite.
     */
    CharacterView(const core::EntityModel& model, const core::SpriteFrame& sprite, const core::CharacterColor& color);
};

#endif // BOMBERMAN_GAME_VIEWS_CHARACTERVIEW_H