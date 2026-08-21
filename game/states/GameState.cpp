#include "GameState.h"

#include "ConcreteFactory.h"
#include "ConcreteRenderer.h"
#include "GameEvent.h"
#include "GameOutcome.h"
#include "GameOverState.h"
#include "HighScores.h"
#include "StateManager.h"
#include "Stopwatch.h"
#include "views/CharacterView.h"

#include <stdexcept>

namespace game {
namespace {
// Where the round's final score is recorded, and where the menu's top-5 list is read from.
// Runtime-generated, not checked into source control (see .gitignore).
constexpr const char* highScoresPath = "highscores.txt";
} // namespace

GameState::GameState(const std::shared_ptr<sf::RenderWindow>& window, StateManager& stateManager)
    : State(window, stateManager), score(std::make_shared<core::Score>()),
      world(std::make_shared<ConcreteFactory>("assets/sprites/spritesheet.png", core::Vector2(16.f, 16.f),
                                              core::Vector2(), core::Vector2(1, 1)),
            "assets/worlds/main.txt", score),
      playerId(world.getPlayerId().value()), renderer(*window) {
    renderer.setViewportSize(
        core::Vector2(static_cast<float>(window->getSize().x), static_cast<float>(window->getSize().y)));

    if (!font.loadFromFile("assets/fonts/arcadeclassic.ttf")) {
        throw std::runtime_error("Failed to load font: assets/fonts/arcadeclassic.ttf");
    }

    scoreText = sf::Text("SCORE 0", font, 24);
    scoreText.setFillColor(sf::Color(188, 190, 0));
    scoreText.setOutlineColor(sf::Color(110, 0, 64));
    scoreText.setOutlineThickness(2.f);
    scoreText.setPosition(10.f, 10.f);
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
    const float deltaTime = core::Stopwatch::getInstance().getDeltaTime();

    // Keep ticking the world even once the outcome is decided, so the last character's death
    // animation actually plays out on screen instead of being cut off by an instant transition to
    // GameOverState.
    world.update(deltaTime);

    if (!outcomeDecided) {
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

        world.moveCharacter(playerId, direction, deltaTime);

        // Time-alive should stop accruing once the round is decided -- not keep climbing through
        // the grace period below, which exists purely to let the death animation finish playing.
        score->tick(deltaTime);
    }

    scoreText.setString("SCORE " + std::to_string(score->getPoints()));

    if (!outcomeDecided) {
        if (const core::GameOutcome outcome = world.getOutcome(); outcome != core::GameOutcome::InProgress) {
            outcomeDecided = true;
            outcomeWasWin = outcome == core::GameOutcome::PlayerWon;

            score->update(core::GameEvent{outcomeWasWin ? core::GameEventType::GameWon : core::GameEventType::GameLost,
                                          core::CharacterColor::White});

            core::HighScores highScores(highScoresPath);
            highScores.record(score->getPoints());
        }
        return;
    }

    outcomeElapsed += deltaTime;
    if (outcomeElapsed >= CharacterView::deathAnimationDuration) {
        stateManager.pushState(std::make_unique<GameOverState>(window, stateManager, outcomeWasWin, score->getPoints()));
    }
}

void GameState::render() {
    world.render(renderer);
    window->draw(scoreText);
}

} // namespace game
