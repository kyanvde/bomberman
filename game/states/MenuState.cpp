#include "MenuState.h"

#include <cmath>
#include <stdexcept>

#include "GameState.h"
#include "StateManager.h"

MenuState::MenuState(const std::shared_ptr<sf::RenderWindow>& window, StateManager& stateManager)
    : State(window, stateManager) {
    if (!font.loadFromFile("assets/fonts/arcadeclassic.ttf")) {
        throw std::runtime_error("Failed to load font: assets/fonts/arcadeclassic.ttf");
    }

    // Title setup
    title = sf::Text("BOMBERMAN", font, baseTitleCharacterSize);
    title.setFillColor(sf::Color(188, 190, 0));
    title.setOutlineColor(sf::Color(110, 0, 64));
    title.setOutlineThickness(baseTitleOutlineThickness);

    // Play button setup
    playButton = sf::Text("PLAY", font, basePlayButtonCharacterSize);
    playButton.setFillColor(sf::Color(188, 190, 0));
    playButton.setOutlineColor(sf::Color(110, 0, 64));
    playButton.setOutlineThickness(basePlayButtonOutlineThickness);

    baseWindowHeight = window->getSize().y;

    layout(window->getSize());
}


void MenuState::layout(const sf::Vector2u& size) {
    const float scale = static_cast<float>(size.y) / static_cast<float>(baseWindowHeight);
    
    // Title
    title.setCharacterSize(std::lround(static_cast<float>(baseTitleCharacterSize) * scale));
    title.setOutlineThickness(baseTitleOutlineThickness * scale);

    const sf::FloatRect titleBounds = title.getLocalBounds();

    title.setOrigin(
        titleBounds.left + titleBounds.width * 0.5f,
        titleBounds.top + titleBounds.height * 0.5f
    );

    title.setPosition(
        static_cast<float>(size.x) * 0.5f,
        static_cast<float>(size.y) * 0.25f
    );

    // Play button
    playButton.setCharacterSize(std::lround(static_cast<float>(basePlayButtonCharacterSize) * scale));
    playButton.setOutlineThickness(basePlayButtonOutlineThickness * scale);

    const sf::FloatRect playBounds = playButton.getLocalBounds();

    playButton.setOrigin(
        playBounds.left + playBounds.width * 0.5f,
        playBounds.top + playBounds.height * 0.5f
    );

    playButton.setPosition(
        static_cast<float>(size.x) * 0.5f,
        static_cast<float>(size.y) * 0.8f
    );
}


void MenuState::processEvent(const sf::Event& event) {
    if (event.type != sf::Event::MouseButtonPressed)
        return;

    if (event.mouseButton.button != sf::Mouse::Left)
        return;

    const sf::Vector2f mousePos(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y));
    if (playButton.getGlobalBounds().contains(mousePos)) {
        stateManager.pushState(std::make_unique<GameState>(window, stateManager));
    }
}


void MenuState::onResize(const sf::Vector2u&, const sf::Vector2u& newSize) {
    layout(newSize);
}


void MenuState::update() {
    // Update menu state
}


void MenuState::render() {
    window->draw(title);
    window->draw(playButton);
}