#include "MenuState.h"
#include <stdio.h>

MenuState::MenuState(std::shared_ptr<sf::RenderWindow> window, StateManager& stateManager)
    : State(window, stateManager) {
    drawables.push_back(std::make_unique<sf::Text>("Menu State", sf::Font(), 30));

}

void MenuState::processEvent(const sf::Event& event) {
    // Process menu events
}

void MenuState::update() {
    // Update menu state
}

void MenuState::render() {
    for (const auto& drawable : drawables) {
        if (drawable) {
            window->draw(*drawable);
        }
    }
}
