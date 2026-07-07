#include "MenuState.h"
#include <stdio.h>

MenuState::MenuState(std::shared_ptr<sf::RenderWindow> window, StateManager& stateManager) : State(window, stateManager) {
    if (!font.loadFromFile("assets/fonts/arcadeclassic.ttf")) {
        printf("Failed to load font\n");
    }

    title = sf::Text("BOMBERMAN", font, baseTitleCharacterSize);
    title.setFillColor(sf::Color(188, 190, 0));
    title.setOutlineColor(sf::Color(110, 0, 64));
    title.setOutlineThickness(baseTitleOutlineThickness);

    baseWindowHeight = window->getSize().y;

    layoutTitle(window->getSize());
}

void MenuState::layoutTitle(const sf::Vector2u& newSize) {
    float scale = static_cast<float>(newSize.y) / static_cast<float>(baseWindowHeight);

    title.setCharacterSize(static_cast<unsigned int>(baseTitleCharacterSize * scale));
    title.setOutlineThickness(baseTitleOutlineThickness * scale);

    const sf::FloatRect bounds = title.getLocalBounds();

    title.setOrigin(bounds.left + bounds.width * 0.5f,
                    bounds.top + bounds.height * 0.5f);

    title.setPosition(newSize.x * 0.5f,
                      newSize.y * 0.25f);
}

void MenuState::processEvent(const sf::Event& event) {
    // Process menu events
}

void MenuState::onResize(const sf::Vector2u& previousSize, const sf::Vector2u& newSize) {
    layoutTitle(newSize);
}

void MenuState::update() {
    // Update menu state
}

void MenuState::render() {
    window->draw(title);
}
