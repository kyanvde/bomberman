#include "MenuState.h"

#include <cmath>
#include <sstream>
#include <stdexcept>

#include "GameState.h"
#include "HighScores.h"
#include "StateManager.h"

namespace game {
namespace {
// Matches GameState.cpp's highScoresPath -- where the round's final score is recorded, and where
// this menu reads the top-5 list from.
constexpr const char* highScoresPath = "highscores.txt";

// The ordinal suffix for a 1-based rank (1st, 2nd, 3rd, 4th, 5th, ...). Only ranks 1-3 get a
// distinct suffix; everything else is "TH" -- more than enough for a top-5 board.
const char* ordinalSuffix(const std::size_t rank) {
    switch (rank) {
        case 1: return "ST";
        case 2: return "ND";
        case 3: return "RD";
        default: return "TH";
    }
}

std::vector<std::string> formatScoreboard(const std::vector<int>& scores) {
    if (scores.empty()) {
        return {"TOP SCORES", "No scores yet"};
    }

    std::vector<std::string> lines;
    lines.push_back("TOP SCORES");
    for (std::size_t i = 0; i < scores.size(); ++i) {
        std::ostringstream line;
        line << (i + 1) << ordinalSuffix(i + 1) << " PLACE " << scores[i];
        lines.push_back(line.str());
    }
    return lines;
}
} // namespace

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

    // Scoreboard setup
    const core::HighScores highScores(highScoresPath);
    for (const std::string& line : formatScoreboard(highScores.getScores())) {
        sf::Text lineText(line, font, baseScoreboardCharacterSize);
        lineText.setFillColor(sf::Color(188, 190, 0));
        lineText.setOutlineColor(sf::Color(110, 0, 64));
        lineText.setOutlineThickness(2.f);
        scoreboardLines.push_back(lineText);
    }

    baseWindowHeight = window->getSize().y;

    layout(window->getSize());
}

void MenuState::layout(const sf::Vector2u& size) {
    const float scale = static_cast<float>(size.y) / static_cast<float>(baseWindowHeight);

    // Title
    title.setCharacterSize(std::lround(static_cast<float>(baseTitleCharacterSize) * scale));
    title.setOutlineThickness(baseTitleOutlineThickness * scale);

    const sf::FloatRect titleBounds = title.getLocalBounds();

    title.setOrigin(titleBounds.left + titleBounds.width * 0.5f, titleBounds.top + titleBounds.height * 0.5f);

    title.setPosition(static_cast<float>(size.x) * 0.5f, static_cast<float>(size.y) * 0.25f);

    // Play button
    playButton.setCharacterSize(std::lround(static_cast<float>(basePlayButtonCharacterSize) * scale));
    playButton.setOutlineThickness(basePlayButtonOutlineThickness * scale);

    const sf::FloatRect playBounds = playButton.getLocalBounds();

    playButton.setOrigin(playBounds.left + playBounds.width * 0.5f, playBounds.top + playBounds.height * 0.5f);

    playButton.setPosition(static_cast<float>(size.x) * 0.5f, static_cast<float>(size.y) * 0.8f);

    // Scoreboard: each line is centered horizontally on its own, then stacked vertically, so lines
    // of different lengths (e.g. "TOP SCORES" vs "1ST PLACE 100") all line up on the same center
    // axis instead of being left-aligned within one shared multi-line block.
    const float lineSpacing = baseScoreboardLineSpacing * scale;
    const float scoreboardCenterX = static_cast<float>(size.x) * 0.5f;
    const float scoreboardTop = static_cast<float>(size.y) * 0.53f;

    for (std::size_t i = 0; i < scoreboardLines.size(); ++i) {
        sf::Text& lineText = scoreboardLines[i];
        lineText.setCharacterSize(std::lround(static_cast<float>(baseScoreboardCharacterSize) * scale));
        lineText.setOutlineThickness(2.f * scale);

        const sf::FloatRect lineBounds = lineText.getLocalBounds();
        lineText.setOrigin(lineBounds.left + lineBounds.width * 0.5f, lineBounds.top + lineBounds.height * 0.5f);
        lineText.setPosition(scoreboardCenterX, scoreboardTop + static_cast<float>(i) * lineSpacing);
    }
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

void MenuState::onResize(const sf::Vector2u&, const sf::Vector2u& newSize) { layout(newSize); }

void MenuState::update() {
    // Update menu state
}

void MenuState::render() {
    window->draw(title);
    window->draw(playButton);
    for (const sf::Text& lineText : scoreboardLines) {
        window->draw(lineText);
    }
}

} // namespace game