#include <memory>
#include <SFML/Graphics.hpp>

class Game {
private:
    void processEvents();

    void render();

    void update();

    std::shared_ptr<sf::RenderWindow> window;
public:
    Game();

    void run();
};
