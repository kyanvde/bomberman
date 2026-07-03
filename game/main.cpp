#include <SFML/Graphics.hpp>
#include "../core/core.h"

int main() {
	temp();
    sf::RenderWindow window(
        sf::VideoMode(800, 600),
        "Bomberman"
    );

    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);
    shape.setPosition(300.f, 200.f);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(shape);
        window.display();
    }

    return 0;
}