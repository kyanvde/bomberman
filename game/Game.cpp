#include "Game.h"

#include "states/MenuState.h"
#include "Stopwatch.h"

void Game::processEvents() {
    sf::Event event{};
    while (window->pollEvent(event)) {
        stateManager.processEvent(event);

        if (event.type == sf::Event::Closed) {
            window->close();   
        }

        if (event.type == sf::Event::Resized) {
            const sf::Vector2u previousSize = window->getSize();
            sf::FloatRect visibleArea(
                0,
                0, 
                static_cast<float>(event.size.width),
                static_cast<float>(event.size.height)
            );

            window->setView(sf::View(visibleArea));
            stateManager.onResize(previousSize, window->getSize());
        }
    }
}

void Game::update() {
    core::Stopwatch::getInstance().tick();
    stateManager.update();
}

void Game::render() {
    window->clear();
    stateManager.render();
    window->display();
}

Game::Game() : window(std::make_shared<sf::RenderWindow>(sf::VideoMode(800, 600), "Bomberman")), stateManager(window) {
    stateManager.pushState(std::make_unique<MenuState>(window, stateManager));
}

void Game::run() {
    while (window->isOpen()) {
        processEvents();
        update();
        render();
    }
}