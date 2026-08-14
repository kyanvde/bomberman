#ifndef BOMBERMAN_GAME_STATE_H
#define BOMBERMAN_GAME_STATE_H

#include <SFML/Graphics.hpp>
#include <memory>

namespace game {

class StateManager;

/**
 * @brief The State class is an abstract base class that represents a state in the game.
 * It provides an interface for handling events, updating game logic, and rendering.
 */
class State {
protected:
    /**
     * @brief A shared pointer to the SFML RenderWindow used for rendering the game.
     */
    std::shared_ptr<sf::RenderWindow> window;

    /**
     * @brief A reference to the StateManager responsible for managing the different states of the game.
     */
    StateManager& stateManager;

public:
    /**
     * @brief Constructs a new State object with the specified window and state manager.
     * @param window A shared pointer to the SFML RenderWindow used for rendering the game.
     * @param stateManager A reference to the StateManager responsible for managing the different states of the game.
     */
    State(const std::shared_ptr<sf::RenderWindow>& window, StateManager& stateManager)
        : window(window), stateManager(stateManager) {}

    /**
     * @brief Virtual destructor for the State class.
     */
    virtual ~State() = default;

    /**
     * @brief Processes an event and handles it within the state.
     * @param event The SFML event to be processed.
     */
    virtual void processEvent(const sf::Event& event) = 0;

    /**
     * @brief Handles window resizing by notifying the state of the change in size.
     * @param previousSize The previous size of the window before resizing.
     * @param newSize The new size of the window after resizing.
     */
    virtual void onResize(const sf::Vector2u&, const sf::Vector2u&) {}

    /**
     * @brief Updates the state, allowing it to perform game logic and state transitions.
     */
    virtual void update() = 0;

    /**
     * @brief Renders the state to the window.
     */
    virtual void render() = 0;
};

} // namespace game

#endif // BOMBERMAN_GAME_STATE_H
