#ifndef BOMBERMAN_GAME_GAME_H
#define BOMBERMAN_GAME_GAME_H

#include <memory>
#include <SFML/Graphics.hpp>
#include "StateManager.h"

/**
 * @brief The Game class is responsible for managing the main game loop, handling events, updating game state, and rendering.
 */
class Game {
    /**
     * @brief Processes all pending events from the window, such as user input and window events.
     */
    void processEvents();

    /**
     * @brief Renders the current state of the game to the window.
     */
    void render();

    /**
     * @brief Updates the game state, including game logic and state transitions.
     */
    void update();

    /**
     * @brief A shared pointer to the SFML RenderWindow used for rendering the game.
     */
    std::shared_ptr<sf::RenderWindow> window;

    /**
     * @brief The StateManager instance responsible for managing the different states of the game.
     */
    StateManager stateManager;
public:
    /**
     * @brief Constructs a new Game object, initializing the window and state manager.
     */
    Game();

    /**
     * @brief Runs the main game loop, continuously processing events, updating the game state, and rendering until the window is closed.
     */
    void run();
};

#endif //BOMBERMAN_GAME_GAME_H
