#ifndef BOMBERMAN_CORE_CAMERA_H
#define BOMBERMAN_CORE_CAMERA_H

#include "Vector2.h"

namespace core {

class Camera {
private:
    Vector2 viewportSize;

public:
    explicit Camera(const Vector2& viewportSize = Vector2(0.f, 0.f)) : viewportSize(viewportSize) {}

    void setViewportSize(const Vector2& newViewportSize) {
        viewportSize = newViewportSize;
    }

    [[nodiscard]] const Vector2& getViewportSize() const noexcept {
        return viewportSize;
    }

    [[nodiscard]] Vector2 projectPosition(const Vector2& worldPosition) const noexcept {
        return {
            (worldPosition.x + 1.f) * 0.5f * viewportSize.x,
            (worldPosition.y + 1.f) * 0.5f * viewportSize.y
        };
    }

    [[nodiscard]] Vector2 projectSize(const Vector2& worldSize) const noexcept {
        return {
            worldSize.x * 0.5f * viewportSize.x,
            worldSize.y * 0.5f * viewportSize.y
        };
    }
};

} // namespace core

#endif // BOMBERMAN_CORE_CAMERA_H