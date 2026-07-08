#ifndef BOMBERMAN_GAME_SFMLRENDERER_H
#define BOMBERMAN_GAME_SFMLRENDERER_H

#include "Renderer.h"
#include <SFML/Graphics.hpp>
#include <stdexcept>
#include <unordered_map>

class SfmlRenderer : public core::Renderer {
private:
    sf::RenderWindow& window;
    mutable std::unordered_map<std::string, sf::Texture> textures;

    const sf::Texture& getTexture(const std::string& texturePath) const {
        auto existing = textures.find(texturePath);
        if (existing != textures.end()) {
            return existing->second;
        }

        sf::Texture texture;
        if (!texture.loadFromFile(texturePath)) {
            throw std::runtime_error("Failed to load texture: " + texturePath);
        }

        auto inserted = textures.emplace(texturePath, std::move(texture));
        return inserted.first->second;
    }

public:
    explicit SfmlRenderer(sf::RenderWindow& window) : window(window) {}

    void drawRectangle(const core::Vector2& position, const core::Vector2& size, const core::Color& color) override {
        const sf::Vector2u windowSize = window.getSize();

        sf::RectangleShape shape;
        shape.setPosition(
            (position.x + 1.f) * 0.5f * static_cast<float>(windowSize.x),
            (position.y + 1.f) * 0.5f * static_cast<float>(windowSize.y)
        );
        shape.setSize({
            size.x * 0.5f * static_cast<float>(windowSize.x),
            size.y * 0.5f * static_cast<float>(windowSize.y)
        });
        shape.setFillColor(sf::Color(color.r, color.g, color.b, color.a));

        window.draw(shape);
    }

    void drawSprite(const core::Vector2& position, const core::Vector2& size, const core::SpriteFrame& sprite) override {
        const sf::Vector2u windowSize = window.getSize();
        const sf::Texture& texture = getTexture(sprite.texturePath);

        sf::Sprite sfSprite(texture);

        const int left = static_cast<int>(sprite.margin.x + sprite.cell.x * (sprite.cellSize.x + sprite.spacing.x));
        const int top = static_cast<int>(sprite.margin.y + sprite.cell.y * (sprite.cellSize.y + sprite.spacing.y));
        const int width = static_cast<int>(sprite.cellSize.x);
        const int height = static_cast<int>(sprite.cellSize.y);

        sfSprite.setTextureRect(sf::IntRect(left, top, width, height));

        sfSprite.setPosition(
            (position.x + 1.f) * 0.5f * static_cast<float>(windowSize.x),
            (position.y + 1.f) * 0.5f * static_cast<float>(windowSize.y)
        );

        sfSprite.setScale(
            (size.x * 0.5f * static_cast<float>(windowSize.x)) / static_cast<float>(width),
            (size.y * 0.5f * static_cast<float>(windowSize.y)) / static_cast<float>(height)
        );

        window.draw(sfSprite);
    }
};

#endif // BOMBERMAN_GAME_SFMLRENDERER_H