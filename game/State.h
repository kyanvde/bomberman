#ifndef BOMBERMAN_GAME_STATE_H
#define BOMBERMAN_GAME_STATE_H

#include <memory>
#include <SFML/Graphics.hpp>
#include <vector>

class StateManager;

class State {
protected:
    std::shared_ptr<sf::RenderWindow> window;

    StateManager& stateManager;

    std::vector<std::unique_ptr<sf::Drawable>> drawables;
public:
    State(std::shared_ptr<sf::RenderWindow> window, StateManager& stateManager) 
        : window(window), stateManager(stateManager) {};

    virtual ~State() = default;

    virtual void processEvent(const sf::Event& event) = 0;

    virtual void update() = 0;

    virtual void render() = 0;
};

#endif //BOMBERMAN_GAME_STATE_H
