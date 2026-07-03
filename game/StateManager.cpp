#include "StateManager.h"

void StateManager::pushState(std::unique_ptr<State> state) {
    states.push(std::move(state));
}

std::unique_ptr<State> StateManager::popState() {
    if (states.empty()) {
        return nullptr;
    }

    std::unique_ptr<State> state = std::move(states.top());
    states.pop();
    return state;
}

void StateManager::processEvent(const sf::Event& event) {
    if (!states.empty()) {
        states.top()->processEvent(event);
    }
}

void StateManager::update() {
    if (!states.empty()) {
        states.top()->update();
    }
}

void StateManager::render() {
    if (!states.empty()) {
        states.top()->render();
    }
}

StateManager::StateManager(std::shared_ptr<sf::RenderWindow> window) : window(window) {}