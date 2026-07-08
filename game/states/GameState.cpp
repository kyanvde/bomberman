#include "GameState.h"

#include "ConcreteFactory.h"
#include "SfmlRenderer.h"

GameState::GameState(std::shared_ptr<sf::RenderWindow> window, StateManager& stateManager)
    : State(window, stateManager), world(std::make_shared<ConcreteFactory>("assets/sprites/bomberman.png", core::Vector2(8.f, 8.f)), "assets/worlds/main.txt") {}

void GameState::processEvent(const sf::Event& event) {}


void GameState::onResize(const sf::Vector2u& previousSize, const sf::Vector2u& newSize) {}


void GameState::update() {
}


void GameState::render() {
    SfmlRenderer renderer(*window);
    world.render(renderer);
}