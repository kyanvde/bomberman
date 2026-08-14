#include "GameOverState.h"

#include <cmath>
#include <stdexcept>

#include "MenuState.h"
#include "StateManager.h"

namespace game {

GameOverState::GameOverState(const std::shared_ptr<sf::RenderWindow>& window, StateManager& stateManager,
                             const bool playerWon, const int finalScore)
    : State(window, stateManager) {
    if (!font.loadFromFile("assets/fonts/arcadeclassic.ttf")) {
        throw std::runtime_error("Failed to load font: assets/fonts/arcadeclassic.ttf");
    }

    resultText = sf::Text(playerWon ? "YOU WIN" : "YOU LOSE", font, baseResultCharacterSize);
    resultText.setFillColor(playerWon ? sf::Color(120, 220, 120) : sf::Color(220, 60, 60));
    resultText.setOutlineColor(sf::Color(110, 0, 64));
    resultText.setOutlineThickness(baseResultOutlineThickness);

    scoreText = sf::Text("Final score: " + std::to_string(finalScore), font, baseScoreCharacterSize);
    scoreText.setFillColor(sf::Color(188, 190, 0));

    hintText = sf::Text("Click to return to the menu", font, baseHintCharacterSize);
    hintText.setFillColor(sf::Color(188, 190, 0));

    baseWindowHeight = window->getSize().y;

    layout(window->getSize());
}

void GameOverState::layout(const sf::Vector2u& size) {
    const float scale = static_cast<float>(size.y) / static_cast<float>(baseWindowHeight);

    resultText.setCharacterSize(std::lround(static_cast<float>(baseResultCharacterSize) * scale));
    resultText.setOutlineThickness(baseResultOutlineThickness * scale);

    const sf::FloatRect resultBounds = resultText.getLocalBounds();
    resultText.setOrigin(resultBounds.left + resultBounds.width * 0.5f, resultBounds.top + resultBounds.height * 0.5f);
    resultText.setPosition(static_cast<float>(size.x) * 0.5f, static_cast<float>(size.y) * 0.35f);

    scoreText.setCharacterSize(std::lround(static_cast<float>(baseScoreCharacterSize) * scale));

    const sf::FloatRect scoreBounds = scoreText.getLocalBounds();
    scoreText.setOrigin(scoreBounds.left + scoreBounds.width * 0.5f, scoreBounds.top + scoreBounds.height * 0.5f);
    scoreText.setPosition(static_cast<float>(size.x) * 0.5f, static_cast<float>(size.y) * 0.5f);

    hintText.setCharacterSize(std::lround(static_cast<float>(baseHintCharacterSize) * scale));

    const sf::FloatRect hintBounds = hintText.getLocalBounds();
    hintText.setOrigin(hintBounds.left + hintBounds.width * 0.5f, hintBounds.top + hintBounds.height * 0.5f);
    hintText.setPosition(static_cast<float>(size.x) * 0.5f, static_cast<float>(size.y) * 0.65f);
}

void GameOverState::processEvent(const sf::Event& event) {
    if (event.type != sf::Event::MouseButtonPressed) {
        return;
    }

    if (event.mouseButton.button != sf::Mouse::Left) {
        return;
    }

    // Pushes a fresh MenuState on top rather than popping back to the original one (mirroring how
    // MenuState itself pushes GameState to start a round, never popping): safe and simple within
    // this stack-based State design, avoiding any risk of a state destroying itself mid-callback.
    stateManager.pushState(std::make_unique<MenuState>(window, stateManager));
}

void GameOverState::onResize(const sf::Vector2u&, const sf::Vector2u& newSize) { layout(newSize); }

void GameOverState::update() {
    // No ongoing logic: the round has ended and this state only waits for input.
}

void GameOverState::render() {
    window->draw(resultText);
    window->draw(scoreText);
    window->draw(hintText);
}

} // namespace game
