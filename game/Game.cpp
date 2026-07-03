#include "Game.h"

void Game::processEvents() {
    sf::Event event;
    while (window->pollEvent(event)) {
        //TODO: INSERT STATEMANAGER EVENT HANDLING HERE

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

}

void Game::render() {
    window->clear();
    // TODO: INSERT STATEMANAGER RENDERING HERE
    window->display();
}

Game::Game() : window(std::make_shared<sf::RenderWindow>(sf::VideoMode(800, 600), "Bomberman")) {}

void Game::run() {
    while (window->isOpen()) {
        processEvents();
        update();
        render();
    }
}