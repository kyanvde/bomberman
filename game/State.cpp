#include "State.h"

namespace game {

State::State(const std::shared_ptr<sf::RenderWindow>& window, StateManager& stateManager)
    : window(window), stateManager(stateManager) {}

State::~State() = default;

void State::onResize(const sf::Vector2u&, const sf::Vector2u&) {}

} // namespace game
