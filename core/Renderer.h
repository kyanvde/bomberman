#ifndef BOMBERMAN_CORE_RENDERER_H
#define BOMBERMAN_CORE_RENDERER_H

#include <string>
#include <utility>
#include "Vector2.h"

namespace core {

/**
 * @brief The SpriteFrame struct represents a single frame of a sprite in a spritesheet.
 * It contains information about the texture path, cell size, cell position, margin, and spacing.
 */
struct SpriteFrame {
    /**
     * @brief The path to the texture image file used for rendering the sprite.
     */
    std::string texturePath;

    /**
     * @brief The size of each cell in the spritesheet, representing the dimensions of individual sprites.
     */
    Vector2 cellSize;

    /**
     * @brief The cell coordinates (x, y) in the spritesheet, representing the position of the sprite frame.
     */
    Vector2 cell;

    /**
     * @brief The margin around the sprites in the spritesheet, used to calculate the correct sprite frame.
     */
    Vector2 margin;

    /**
     * @brief The spacing between sprites in the spritesheet, used to calculate the correct sprite frame.
     */
    Vector2 spacing;

    /**
     * @brief Constructs a new SpriteFrame object with default values.
     */
    SpriteFrame();

    /**
     * @brief Constructs a new SpriteFrame object with the specified texture path, cell size, cell position, margin, and spacing.
     * @param texturePath The path to the texture image file.
     * @param cellSize The size of each cell in the spritesheet.
     * @param cell The cell coordinates (x, y) in the spritesheet.
     * @param margin The margin around the sprites in the spritesheet (default is (0, 0)).
     * @param spacing The spacing between sprites in the spritesheet (default is (0, 0)).
     */
    SpriteFrame(std::string  texturePath,
                const Vector2& cellSize,
                const Vector2& cell,
                const Vector2& margin = Vector2(),
                const Vector2& spacing = Vector2())
        : texturePath(std::move(texturePath)), cellSize(cellSize), cell(cell), margin(margin), spacing(spacing) {}
};

/**
 * @brief The Renderer class is an abstract base class that defines the interface for rendering sprites.
 * It provides a pure virtual function for drawing a sprite at a specified position and size.
 */
class Renderer {
public:
    /**
     * @brief Virtual destructor for the Renderer class.
     */
    virtual ~Renderer() = default;

    /**
     * @brief Draws a sprite at the specified position and size using the provided SpriteFrame.
     * @param position The position in world coordinates where the sprite should be drawn.
     * @param size The size in world coordinates of the sprite to be drawn.
     * @param sprite The SpriteFrame containing information about the sprite to be drawn.
     */
    virtual void drawSprite(const Vector2& position, const Vector2& size, const SpriteFrame& sprite) = 0;
};

} // namespace core

#endif // BOMBERMAN_CORE_RENDERER_H