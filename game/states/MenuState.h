#ifndef BOMBERMAN_GAME_STATES_MENUSTATE_H
#define BOMBERMAN_GAME_STATES_MENUSTATE_H

#include "State.h"
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>

namespace game {

/**
 * @brief The MenuState class represents the main menu state of the game.
 * It handles user input, updates the menu logic, and renders the menu interface.
 */
class MenuState final : public State {
    /**
     * @brief The font used for rendering text in the menu.
     */
    sf::Font font;

    /**
     * @brief The title text displayed in the menu.
     */
    sf::Text title;

    /**
     * @brief The play button text displayed in the menu.
     */
    sf::Text playButton;

    /**
     * @brief The persisted top-5 high score list, displayed below the play button, one sf::Text
     * per line. A single multi-line sf::Text left-aligns each line within its own block instead of
     * centering them individually, which looks ragged when lines have different lengths (e.g. "TOP
     * SCORES" vs "1ST PLACE 100") -- rendering each line as its own centered Text avoids that.
     */
    std::vector<sf::Text> scoreboardLines;

    /**
     * @brief The base width of the window used for scaling UI elements.
     */
    unsigned int baseWindowHeight = 0;

    /**
     * @brief The base height of the window used for scaling UI elements.
     */
    unsigned int baseTitleCharacterSize = 100;

    /**
     * @brief The base outline thickness of the title text used for scaling UI elements.
     */
    float baseTitleOutlineThickness = 5.f;

    /**
     * @brief The base character size of the play button text used for scaling UI elements.
     */
    unsigned int basePlayButtonCharacterSize = 60;

    /**
     * @brief The base outline thickness of the play button text used for scaling UI elements.
     */
    float basePlayButtonOutlineThickness = 4.f;

    /**
     * @brief The base character size of the scoreboard text used for scaling UI elements.
     */
    unsigned int baseScoreboardCharacterSize = 28;

    /**
     * @brief The base vertical gap between consecutive scoreboard lines, used for scaling UI elements.
     */
    float baseScoreboardLineSpacing = 34.f;

    /**
     * @brief Lays out the UI elements based on the current window size.
     * @param size The current size of the window.
     */
    void layout(const sf::Vector2u& size);

public:
    /**
     * @brief Processes an event and handles it within the menu state.
     * @param event The SFML event to be processed.
     */
    void processEvent(const sf::Event& event) override;

    /**
     * @brief Handles window resizing by notifying the menu state of the change in size.
     * @param previousSize The previous size of the window before resizing.
     * @param newSize The new size of the window after resizing.
     */
    void onResize(const sf::Vector2u& previousSize, const sf::Vector2u& newSize) override;

    /**
     * @brief Updates the menu state, allowing it to perform menu logic and state transitions.
     */
    void update() override;

    /**
     * @brief Renders the menu state to the window.
     */
    void render() override;

    /**
     * @brief Constructs a new MenuState object with the specified window and state manager.
     * @param window A shared pointer to the SFML RenderWindow used for rendering the game.
     * @param stateManager A reference to the StateManager responsible for managing the different states of the game.
     */
    MenuState(const std::shared_ptr<sf::RenderWindow>& window, StateManager& stateManager);
};

} // namespace game

#endif // BOMBERMAN_GAME_STATES_MENUSTATE_H