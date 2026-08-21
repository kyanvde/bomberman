#ifndef BOMBERMAN_GAME_STATES_GAMESTATE_H
#define BOMBERMAN_GAME_STATES_GAMESTATE_H

#include <ConcreteRenderer.h>

#include "Score.h"
#include "State.h"
#include "World.h"
#include <SFML/Graphics.hpp>
#include <memory>
#include <set>

namespace game {

/**
 * @brief The GameState class represents the main game state of the application.
 * It handles user input, updates the game world, and renders the game scene.
 */
class GameState final : public State {
    /**
     * @brief This round's Score, created before the world so it can be passed into World's
     * constructor and attached to every entity (including ones loaded from the world file).
     */
    std::shared_ptr<core::Score> score;

    /**
     * @brief The game world that contains all game entities and logic.
     */
    core::World world;

    /**
     * @brief The identifier of the human-controlled player's character, resolved once at construction.
     */
    core::EntityId playerId;

    /**
     * @brief The SFML renderer used for rendering the game world.
     */
    ConcreteRenderer renderer;

    /**
     * @brief A set of currently held keyboard keys for smooth continuous movement.
     */
    std::set<sf::Keyboard::Key> heldKeys;

    /**
     * @brief The font used for rendering the score HUD.
     */
    sf::Font font;

    /**
     * @brief The current-score text drawn each frame.
     */
    sf::Text scoreText;

    /**
     * @brief Whether the round's outcome (win or loss) has been decided yet. Once true, the world
     * keeps ticking -- so the last character's death animation can actually finish playing on
     * screen -- but player input and further outcome checks are no longer applied.
     */
    bool outcomeDecided = false;

    /**
     * @brief Whether the Player won, valid only once outcomeDecided is true. Cached at the moment
     * the outcome is decided, so GameOverState is constructed with the same result once the grace
     * period below has elapsed.
     */
    bool outcomeWasWin = false;

    /**
     * @brief How long the outcome has been decided for, in seconds. Once this reaches
     * CharacterView::deathAnimationDuration, GameOverState is pushed.
     */
    float outcomeElapsed = 0.f;

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

} // namespace game

#endif // BOMBERMAN_GAME_STATES_GAMESTATE_H
