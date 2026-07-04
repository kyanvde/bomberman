#ifndef BOMBERMAN_GAME_STATES_MENUSTATE_H
#define BOMBERMAN_GAME_STATES_MENUSTATE_H

#include "../State.h"
#include <memory>
#include <SFML/Graphics.hpp>

class MenuState : public State {
public:
    void processEvent(const sf::Event& event) override;

    void update() override;

    void render() override;

    MenuState(std::shared_ptr<sf::RenderWindow> window, StateManager& stateManager);
};
#endif // BOMBERMAN_GAME_STATES_MENUSTATE_H