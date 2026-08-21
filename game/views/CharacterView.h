#ifndef BOMBERMAN_GAME_VIEWS_CHARACTERVIEW_H
#define BOMBERMAN_GAME_VIEWS_CHARACTERVIEW_H

#include "Animation.h"
#include "EntityView.h"

namespace core {
enum class CharacterColor;
}

namespace game {

/**
 * @brief The CharacterView class represents the visual representation of a character entity in the game.
 * It inherits from EntityView and provides specific rendering for character entities.
 */
class CharacterView : public EntityView {
    core::CharacterColor color;

public:
    /**
     * @brief How many frames the death animation plays before holding on its last frame.
     */
    static constexpr std::size_t deathFrameCount = 7;

    /**
     * @brief How long each death animation frame is shown, in seconds.
     */
    static constexpr float deathFrameDuration = 0.2f;

    /**
     * @brief The death animation's total playtime, in seconds. Exposed so GameState can hold the
     * win/lose screen off the last dying character's death animation actually finishing, rather
     * than the two independently drifting out of sync over time.
     */
    static constexpr float deathAnimationDuration = static_cast<float>(deathFrameCount) * deathFrameDuration;

    /**
     * @brief Constructs a new CharacterView object with the specified EntityModel and sprite frame.
     * @param model A reference to the EntityModel that this view represents.
     * @param sprite The sprite frame used for rendering the character entity.
     * @param color The color of the character, which can be used to customize the appearance of the character sprite.
     */
    CharacterView(const core::EntityModel& model, const core::SpriteFrame& sprite, const core::CharacterColor& color);
};

} // namespace game

#endif // BOMBERMAN_GAME_VIEWS_CHARACTERVIEW_H