#ifndef BOMBERMAN_GAME_STATES_GAMEOVERSTATE_H
#define BOMBERMAN_GAME_STATES_GAMEOVERSTATE_H

#include "State.h"
#include <SFML/Graphics.hpp>

namespace game {

/**
 * @brief The GameOverState class represents the end-of-round screen shown after the Player has
 * won or lost. It displays the outcome and returns to the main menu on input.
 */
class GameOverState final : public State {
    /**
     * @brief The font used for rendering text in this state.
     */
    sf::Font font;

    /**
     * @brief The win/lose result text displayed to the player.
     */
    sf::Text resultText;

    /**
     * @brief A hint prompting the player how to return to the menu.
     */
    sf::Text hintText;

    /**
     * @brief The base window height used for scaling UI elements.
     */
    unsigned int baseWindowHeight = 0;

    /**
     * @brief The base character size of the result text used for scaling UI elements.
     */
    unsigned int baseResultCharacterSize = 80;

    /**
     * @brief The base outline thickness of the result text used for scaling UI elements.
     */
    float baseResultOutlineThickness = 5.f;

    /**
     * @brief The base character size of the hint text used for scaling UI elements.
     */
    unsigned int baseHintCharacterSize = 30;

    /**
     * @brief Lays out the UI elements based on the current window size.
     * @param size The current size of the window.
     */
    void layout(const sf::Vector2u& size);

public:
    /**
     * @brief Processes an event and handles it within the game-over state. Any left click returns
     * to the main menu.
     * @param event The SFML event to be processed.
     */
    void processEvent(const sf::Event& event) override;

    /**
     * @brief Handles window resizing by notifying the game-over state of the change in size.
     * @param previousSize The previous size of the window before resizing.
     * @param newSize The new size of the window after resizing.
     */
    void onResize(const sf::Vector2u& previousSize, const sf::Vector2u& newSize) override;

    /**
     * @brief Updates the game-over state. There is no ongoing logic here; the round has ended.
     */
    void update() override;

    /**
     * @brief Renders the game-over state to the window.
     */
    void render() override;

    /**
     * @brief Constructs a new GameOverState object with the specified window, state manager, and
     * outcome.
     * @param window A shared pointer to the SFML RenderWindow used for rendering the game.
     * @param stateManager A reference to the StateManager responsible for managing the different
     * states of the game.
     * @param playerWon True if the Player won the round, false if the Player lost.
     */
    GameOverState(const std::shared_ptr<sf::RenderWindow>& window, StateManager& stateManager, bool playerWon);
};

} // namespace game

#endif // BOMBERMAN_GAME_STATES_GAMEOVERSTATE_H
