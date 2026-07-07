#ifndef BOMBERMAN_GAME_STATES_GAMESTATE_H
#define BOMBERMAN_GAME_STATES_GAMESTATE_H

#include "../State.h"
#include <memory>
#include <SFML/Graphics.hpp>

class GameState : public State {
private:
    
public:
    void processEvent(const sf::Event& event) override;

    void onResize(const sf::Vector2u& previousSize, const sf::Vector2u& newSize) override;

    void update() override;

    void render() override;

    GameState(std::shared_ptr<sf::RenderWindow> window, StateManager& stateManager);
};

#endif // BOMBERMAN_GAME_STATES_GAMESTATE_H