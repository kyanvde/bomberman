#include "AbstractRenderer.h"

#include <utility>

namespace core {

SpriteFrame::SpriteFrame(std::string texturePath, const Vector2& cellSize, const Vector2& cell,
                         const Vector2& margin, const Vector2& spacing)
    : texturePath(std::move(texturePath)), cellSize(cellSize), cell(cell), margin(margin), spacing(spacing) {}

AbstractRenderer::~AbstractRenderer() = default;

} // namespace core
