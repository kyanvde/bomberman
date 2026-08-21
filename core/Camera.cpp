#include "Camera.h"

namespace core {

Camera::Camera(const Vector2& viewportSize) : viewportSize(viewportSize) {}

void Camera::setViewportSize(const Vector2& newViewportSize) { viewportSize = newViewportSize; }

const Vector2& Camera::getViewportSize() const noexcept { return viewportSize; }

Vector2 Camera::projectPosition(const Vector2& worldPosition) const noexcept {
    return {(worldPosition.x + 1.f) * 0.5f * viewportSize.x, (worldPosition.y + 1.f) * 0.5f * viewportSize.y};
}

Vector2 Camera::projectSize(const Vector2& worldSize) const noexcept {
    return {worldSize.x * 0.5f * viewportSize.x, worldSize.y * 0.5f * viewportSize.y};
}

} // namespace core
