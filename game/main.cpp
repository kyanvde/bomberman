#include "Game.h"

#include <exception>
#include <iostream>

/**
 * @brief The main entry point of the application.
 * It creates a Game instance and runs the main game loop.
 * Any exception raised during setup or play (e.g. a missing asset file) is
 * caught here so the application exits cleanly instead of crashing.
 * @return EXIT_SUCCESS on successful execution, EXIT_FAILURE if an error occurred.
 */
int main() {
    try {
        game::Game game;
        game.run();
    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << '\n';
        return EXIT_FAILURE;
    } catch (...) {
        std::cerr << "Fatal error: an unknown exception occurred\n";
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
