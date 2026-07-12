#ifndef BOMBERMAN_GAME_SFMLRENDERER_H
#define BOMBERMAN_GAME_SFMLRENDERER_H

#include "Camera.h"
#include "Renderer.h"
#include <SFML/Graphics.hpp>
#include <stdexcept>
#include <unordered_map>

class SfmlRenderer : public core::Renderer {
private:
    sf::RenderWindow& window;
    mutable std::unordered_map<std::string, sf::Texture> textures;
    core::Camera camera;

    const sf::Texture& getTexture(const std::string& texturePath) const;

public:
    explicit SfmlRenderer(sf::RenderWindow& window);

    void setViewportSize(const core::Vector2& viewportSize);

    void drawSprite(const core::Vector2& position, const core::Vector2& size, const core::SpriteFrame& sprite) override;
};

#endif // BOMBERMAN_GAME_SFMLRENDERER_H