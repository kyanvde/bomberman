#ifndef BOMBERMAN_GAME_STATES_GAMESTATE_H
#define BOMBERMAN_GAME_STATES_GAMESTATE_H

#include <ConcreteRenderer.h>

#include "State.h"
#include "World.h"
#include <SFML/Graphics.hpp>
#include <memory>
#include <set>

/**
 * @brief The GameState class represents the main game state of the application.
 * It handles user input, updates the game world, and renders the game scene.
 */
class GameState final : public State {
    /**
     * @brief The game world that contains all game entities and logic.
     */
    core::World world;

    /**
     * @brief The SFML renderer used for rendering the game world.
     */
    ConcreteRenderer renderer;

    /**
     * @brief A set of currently held keyboard keys for smooth continuous movement.
     */
    std::set<sf::Keyboard::Key> heldKeys;

public:
    /**
     * @brief Processes an event and handles it within the game state.
     * @param event The SFML event to be processed.
     */
    void processEvent(const sf::Event& event) override;

    /**
     * @brief Handles window resizing by notifying the game state of the change in size.
     * @param previousSize The previous size of the window before resizing.
     * @param newSize The new size of the window after resizing.
     */
    void onResize(const sf::Vector2u& previousSize, const sf::Vector2u& newSize) override;

    /**
     * @brief Updates the game state, including game logic and state transitions.
     */
    void update() override;

    /**
     * @brief Renders the game state to the window.
     */
    void render() override;

    /**
     * @brief Constructs a new GameState object with the specified window and state manager.
     * @param window A shared pointer to the SFML RenderWindow used for rendering the game.
     * @param stateManager A reference to the StateManager responsible for managing the different states of the game.
     */
    GameState(const std::shared_ptr<sf::RenderWindow>& window, StateManager& stateManager);
};

#endif // BOMBERMAN_GAME_STATES_GAMESTATE_H