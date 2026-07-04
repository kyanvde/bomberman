#ifndef BOMBERMAN_GAME_STATEMANAGER_H
#define BOMBERMAN_GAME_STATEMANAGER_H

#include <memory>
#include "State.h"
#include <SFML/Graphics.hpp>
#include <stack>

class StateManager {
private:
    std::shared_ptr<sf::RenderWindow> window;

    std::stack<std::unique_ptr<State>> states;
public:
    void pushState(std::unique_ptr<State> state);

    std::unique_ptr<State> popState();

    void processEvent(const sf::Event& event);

    void update();

    void render();

    StateManager(std::shared_ptr<sf::RenderWindow> window);
};

#endif //BOMBERMAN_GAME_STATEMANAGER_H