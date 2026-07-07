#ifndef BOMBERMAN_GAME_STATES_MENUSTATE_H
#define BOMBERMAN_GAME_STATES_MENUSTATE_H

#include "../State.h"
#include <memory>
#include <SFML/Graphics.hpp>

class MenuState : public State {
private:
    sf::Font font;
    sf::Text title;
    sf::Text playButton;

    unsigned int baseWindowHeight = 0;

    unsigned int baseTitleCharacterSize = 100;
    float baseTitleOutlineThickness = 5.f;

    unsigned int basePlayButtonCharacterSize = 60;
    float basePlayButtonOutlineThickness = 4.f;

    void layout(const sf::Vector2u& size);
public:
    void processEvent(const sf::Event& event) override;

    void onResize(const sf::Vector2u& previousSize, const sf::Vector2u& newSize) override;

    void update() override;

    void render() override;

    MenuState(std::shared_ptr<sf::RenderWindow> window, StateManager& stateManager);
};

#endif // BOMBERMAN_GAME_STATES_MENUSTATE_H