#ifndef BOMBERMAN_GAME_GAME_H
#define BOMBERMAN_GAME_GAME_H

#include <memory>
#include <SFML/Graphics.hpp>
#include "StateManager.h"

class Game {
private:
    void processEvents();

    void render();

    void update();

    std::shared_ptr<sf::RenderWindow> window;
    StateManager stateManager;
public:
    Game();

    void run();
};

#endif //BOMBERMAN_GAME_GAME_H
