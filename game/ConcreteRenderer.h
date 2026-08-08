#ifndef BOMBERMAN_GAME_SFMLRENDERER_H
#define BOMBERMAN_GAME_SFMLRENDERER_H

#include "Camera.h"
#include "AbstractRenderer.h"
#include <SFML/Graphics.hpp>
#include <unordered_map>

/**
 * @brief The ConcreteRenderer class is a concrete implementation of the core::Renderer interface using SFML for rendering.
 * It provides functionality to draw sprites on an SFML RenderWindow, manage textures, and handle camera projection.
 */
class ConcreteRenderer final : public core::AbstractRenderer {
    /**
     * @brief A reference to the SFML RenderWindow used for rendering the game.
     */
    sf::RenderWindow& window;

    /**
     * @brief A mutable unordered map that caches loaded textures to avoid reloading them from disk.
     * The key is the texture path, and the value is the corresponding SFML texture.
     */
    mutable std::unordered_map<std::string, sf::Texture> textures;

    /**
     * @brief The camera used for projecting world coordinates to screen coordinates.
     */
    core::Camera camera;

    /**
     * @brief Retrieves the SFML texture associated with the given texture path.
     * If the texture is not already loaded, it will be loaded from disk and cached.
     * @param texturePath The path to the texture file.
     * @return A reference to the SFML texture associated with the given path.
     * @throws std::runtime_error if the texture fails to load from disk.
     */
    const sf::Texture& getTexture(const std::string& texturePath) const;

public:
    /**
     * @brief Constructs a new ConcreteRenderer object with the specified SFML RenderWindow.
     * @param window A reference to the SFML RenderWindow used for rendering the game.
     */
    explicit ConcreteRenderer(sf::RenderWindow& window);

    /**
     * @brief Sets the viewport size for the camera, allowing it to project world coordinates to screen coordinates correctly.
     * @param viewportSize The new size of the viewport in pixels.
     */
    void setViewportSize(const core::Vector2& viewportSize);

    /**
     * @brief Draws a sprite on the screen at the specified position and size using the provided sprite frame.
     * @param position The position in world coordinates where the sprite should be drawn.
     * @param size The size of the sprite in world coordinates.
     * @param sprite The sprite frame containing texture information and cell details for rendering.
     */
    void drawSprite(const core::Vector2& position, const core::Vector2& size, const core::SpriteFrame& sprite) override;
};

#endif // BOMBERMAN_GAME_SFMLRENDERER_H