#include "GameState.h"

#include "ConcreteFactory.h"
#include "ConcreteRenderer.h"
#include "GameOutcome.h"
#include "GameOverState.h"
#include "StateManager.h"
#include "Stopwatch.h"

namespace game {

GameState::GameState(const std::shared_ptr<sf::RenderWindow>& window, StateManager& stateManager)
    : State(window, stateManager),
      world(std::make_shared<ConcreteFactory>("assets/sprites/spritesheet.png", core::Vector2(16.f, 16.f),
                                              core::Vector2(), core::Vector2(1, 1)),
            "assets/worlds/main.txt"),
      playerId(world.getPlayerId().value()), renderer(*window) {
    renderer.setViewportSize(
        core::Vector2(static_cast<float>(window->getSize().x), static_cast<float>(window->getSize().y)));
}

void GameState::processEvent(const sf::Event& event) {
    if (event.type == sf::Event::KeyPressed) {
        heldKeys.insert(event.key.code);

        if (event.key.code == sf::Keyboard::Space) {
            world.placeBomb(playerId);
        }

    } else if (event.type == sf::Event::KeyReleased) {
        heldKeys.erase(event.key.code);
    }
}

void GameState::onResize(const sf::Vector2u&, const sf::Vector2u& newSize) {
    renderer.setViewportSize(core::Vector2(static_cast<float>(newSize.x), static_cast<float>(newSize.y)));
}

void GameState::update() {
    core::Vector2 direction(0.f, 0.f);

    for (const auto& key : heldKeys) {
        if (key == sf::Keyboard::Left || key == sf::Keyboard::A) {
            direction.x -= 1.f;
        } else if (key == sf::Keyboard::Right || key == sf::Keyboard::D) {
            direction.x += 1.f;
        } else if (key == sf::Keyboard::Up || key == sf::Keyboard::W) {
            direction.y -= 1.f;
        } else if (key == sf::Keyboard::Down || key == sf::Keyboard::S) {
            direction.y += 1.f;
        }
    }

    const float deltaTime = core::Stopwatch::getInstance().getDeltaTime();
    world.update(deltaTime);
    world.moveCharacter(playerId, direction, deltaTime);

    if (const core::GameOutcome outcome = world.getOutcome(); outcome != core::GameOutcome::InProgress) {
        stateManager.pushState(
            std::make_unique<GameOverState>(window, stateManager, outcome == core::GameOutcome::PlayerWon));
    }
}

void GameState::render() { world.render(renderer); }

} // namespace game