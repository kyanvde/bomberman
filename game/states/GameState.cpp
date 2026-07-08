#include "GameState.h"

#include "ConcreteFactory.h"

GameState::GameState(std::shared_ptr<sf::RenderWindow> window, StateManager& stateManager)
    : State(window, stateManager), world(std::make_shared<ConcreteFactory>(), "assets/worlds/main.txt") {}

void GameState::processEvent(const sf::Event& event) {}


void GameState::onResize(const sf::Vector2u& previousSize, const sf::Vector2u& newSize) {}


void GameState::update() {
    // Update game state
}


void GameState::render() {
    // Render game state
}