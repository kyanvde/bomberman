#include "GameState.h"

#include "ConcreteFactory.h"
#include "ConcreteRenderer.h"
#include "Stopwatch.h"

GameState::GameState(const std::shared_ptr<sf::RenderWindow>& window, StateManager& stateManager)
    : State(window, stateManager),
      world(std::make_shared<ConcreteFactory>("assets/sprites/bomberman.png", core::Vector2(8.f, 8.f)), "assets/worlds/main.txt"),
      renderer(*window) {
    renderer.setViewportSize(core::Vector2(static_cast<float>(window->getSize().x), static_cast<float>(window->getSize().y)));
}

void GameState::processEvent(const sf::Event& event) {
    core::Vector2 direction(0.f, 0.f);
    
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Left || event.key.code == sf::Keyboard::A) {
            direction.x -= 1.f;
        } else if (event.key.code == sf::Keyboard::Right || event.key.code == sf::Keyboard::D) {
            direction.x += 1.f;
        } else if (event.key.code == sf::Keyboard::Up || event.key.code == sf::Keyboard::W) {
            direction.y -= 1.f;
        } else if (event.key.code == sf::Keyboard::Down || event.key.code == sf::Keyboard::S) {
            direction.y += 1.f;
        }
    }

    world.movePlayer(direction, core::Stopwatch::getInstance().getDeltaTime());
}

void GameState::onResize(const sf::Vector2u&, const sf::Vector2u& newSize) {
    renderer.setViewportSize(core::Vector2(static_cast<float>(newSize.x), static_cast<float>(newSize.y)));
}

void GameState::update() {}

void GameState::render() {
    world.render(renderer);
}