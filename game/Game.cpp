#include "Game.h"

void Game::processEvents() {
    sf::Event event;
    while (window->pollEvent(event)) {
        stateManager.processEvent(event);

        if (event.type == sf::Event::Closed) {
            window->close();   
        }

        if (event.type == sf::Event::Resized) {
            sf::FloatRect visibleArea(
                0,
                0, 
                static_cast<float>(event.size.width),
                static_cast<float>(event.size.height)
            );

            window->setView(sf::View(visibleArea));
        }
    }
}

void Game::update() {
    // TODO: Start new game tick
    stateManager.update();
}

void Game::render() {
    window->clear();
    stateManager.render();
    window->display();
}

Game::Game() : window(std::make_shared<sf::RenderWindow>(sf::VideoMode(800, 600), "Bomberman")), stateManager(window) {}

void Game::run() {
    while (window->isOpen()) {
        processEvents();
        update();
        render();
    }
}