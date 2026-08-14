#include "Game.h"

/**
 * @brief The main entry point of the application.
 * It creates a Game instance and runs the main game loop.
 * @return EXIT_SUCCESS on successful execution.
 */
int main() {
    game::Game game;
    game.run();

    return EXIT_SUCCESS;
}