#ifndef BOMBERMAN_CORE_RENDERER_H
#define BOMBERMAN_CORE_RENDERER_H

#include <string>
#include "Vector2.h"

namespace core {

struct SpriteFrame {
    std::string texturePath;
    Vector2 cellSize;
    Vector2 cell;
    Vector2 margin;
    Vector2 spacing;

    SpriteFrame()
        : texturePath(), cellSize(), cell(), margin(), spacing() {}

    SpriteFrame(const std::string& texturePath,
                const Vector2& cellSize,
                const Vector2& cell,
                const Vector2& margin = Vector2(),
                const Vector2& spacing = Vector2())
        : texturePath(texturePath), cellSize(cellSize), cell(cell), margin(margin), spacing(spacing) {}
};

class Renderer {
public:
    virtual ~Renderer() = default;

    virtual void drawSprite(const Vector2& position, const Vector2& size, const SpriteFrame& sprite) = 0;
};

} // namespace core

#endif // BOMBERMAN_CORE_RENDERER_H