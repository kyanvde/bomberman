#ifndef BOMBERMAN_GAME_STATEMANAGER_H
#define BOMBERMAN_GAME_STATEMANAGER_H

#include "State.h"
#include <SFML/Graphics.hpp>
#include <memory>
#include <stack>

/**
 * @brief The StateManager class is responsible for managing the different states of the game.
 * It allows pushing and popping states, processing events, handling window resizing, updating the current state, and
 * rendering.
 */
class StateManager {
    /**
     * @brief A shared pointer to the SFML RenderWindow used for rendering the game.
     */
    std::shared_ptr<sf::RenderWindow> window;

    /**
     * @brief A stack of unique pointers to the current states of the game.
     * The top of the stack represents the active state.
     */
    std::stack<std::unique_ptr<State>> states;

public:
    /**
     * @brief Pushes a new state onto the stack, making it the active state.
     * @param state A unique pointer to the new state to be pushed onto the stack.
     */
    void pushState(std::unique_ptr<State> state);

    /**
     * @brief Pops the current active state from the stack, returning it.
     * @return A unique pointer to the popped state.
     */
    std::unique_ptr<State> popState();

    /**
     * @brief Processes an event and forwards it to the current active state for handling.
     * @param event The SFML event to be processed.
     */
    void processEvent(const sf::Event& event);

    /**
     * @brief Handles window resizing by notifying the current active state of the change in size.
     * @param previousSize The previous size of the window before resizing.
     * @param newSize The new size of the window after resizing.
     */
    void onResize(const sf::Vector2u& previousSize, const sf::Vector2u& newSize);

    /**
     * @brief Updates the current active state, allowing it to perform game logic and state transitions.
     */
    void update();

    /**
     * @brief Renders the current active state to the window.
     */
    void render();

    /**
     * @brief Constructs a new StateManager object with the specified window.
     * @param window A shared pointer to the SFML RenderWindow used for rendering the game.
     */
    explicit StateManager(std::shared_ptr<sf::RenderWindow> window);
};

#endif // BOMBERMAN_GAME_STATEMANAGER_H