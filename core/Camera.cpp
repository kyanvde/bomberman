#include "Camera.h"

#include <algorithm>

namespace core {

Camera::Camera(const Vector2& viewportSize) : viewportSize(viewportSize) {}

void Camera::setViewportSize(const Vector2& newViewportSize) { viewportSize = newViewportSize; }

const Vector2& Camera::getViewportSize() const noexcept { return viewportSize; }

Camera::Projection Camera::projection() const noexcept {
    // The world spans 2 world-units (-1 to 1) along each axis, so half the viewport's smaller
    // dimension is exactly the pixel scale that keeps both axes uniform.
    const float scale = std::min(viewportSize.x, viewportSize.y) * 0.5f;
    return {scale, (viewportSize.x - scale * 2.f) * 0.5f, (viewportSize.y - scale * 2.f) * 0.5f};
}

Vector2 Camera::projectPosition(const Vector2& worldPosition) const noexcept {
    const Projection p = projection();
    return {p.offsetX + (worldPosition.x + 1.f) * p.scale, p.offsetY + (worldPosition.y + 1.f) * p.scale};
}

Vector2 Camera::projectSize(const Vector2& worldSize) const noexcept {
    const Projection p = projection();
    return {worldSize.x * p.scale, worldSize.y * p.scale};
}

} // namespace core
