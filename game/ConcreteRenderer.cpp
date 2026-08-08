#include "ConcreteRenderer.h"

namespace {
    core::Vector2 toVector2(const sf::Vector2u& size) {
        return {static_cast<float>(size.x), static_cast<float>(size.y)};
    }
}

const sf::Texture& ConcreteRenderer::getTexture(const std::string& texturePath) const {
    if (const auto existing = textures.find(texturePath); existing != textures.end()) {
        return existing->second;
    }

    sf::Texture texture;
    if (!texture.loadFromFile(texturePath)) {
        throw std::runtime_error("Failed to load texture: " + texturePath);
    }

    auto inserted = textures.emplace(texturePath, std::move(texture));
    return inserted.first->second;
}

ConcreteRenderer::ConcreteRenderer(sf::RenderWindow& window) : window(window), camera(toVector2(window.getSize())) {}

void ConcreteRenderer::setViewportSize(const core::Vector2& viewportSize) {
    camera.setViewportSize(viewportSize);
}

void ConcreteRenderer::drawSprite(const core::Vector2& position, const core::Vector2& size, const core::SpriteFrame& sprite) {
    const sf::Texture& texture = getTexture(sprite.texturePath);

    sf::Sprite sfSprite(texture);

    const int left = static_cast<int>(sprite.margin.x + sprite.cell.x * (sprite.cellSize.x + sprite.spacing.x));
    const int top = static_cast<int>(sprite.margin.y + sprite.cell.y * (sprite.cellSize.y + sprite.spacing.y));
    const int width = static_cast<int>(sprite.cellSize.x);
    const int height = static_cast<int>(sprite.cellSize.y);

    sfSprite.setTextureRect(sf::IntRect(left, top, width, height));

    const core::Vector2 projectedPosition = camera.projectPosition(position);
    const core::Vector2 projectedSize = camera.projectSize(size);

    sfSprite.setPosition(projectedPosition.x, projectedPosition.y);

    sfSprite.setScale(
        projectedSize.x / static_cast<float>(width),
        projectedSize.y / static_cast<float>(height)
    );

    window.draw(sfSprite);
}