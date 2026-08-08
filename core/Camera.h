#ifndef BOMBERMAN_CORE_CAMERA_H
#define BOMBERMAN_CORE_CAMERA_H

#include "Vector2.h"

namespace core {

/**
 * @brief The Camera class represents a camera in the game world.
 * It provides methods to project world coordinates to screen coordinates based on the viewport size.
 */
class Camera {
    /**
     * @brief The size of the viewport in screen coordinates.
     */
    Vector2 viewportSize;

public:
    /**
     * @brief Constructs a new Camera object with the specified viewport size.
     * @param viewportSize The size of the viewport in screen coordinates (default is (0, 0)).
     */
    explicit Camera(const Vector2& viewportSize = Vector2(0.f, 0.f)) : viewportSize(viewportSize) {}

    /**
     * @brief Sets the size of the viewport.
     * @param newViewportSize The new size of the viewport in screen coordinates.
     */
    void setViewportSize(const Vector2& newViewportSize) {
        viewportSize = newViewportSize;
    }

    /**
     * @brief Retrieves the size of the viewport.
     * @return A constant reference to the Vector2 representing the viewport size.
     */
    [[nodiscard]] const Vector2& getViewportSize() const noexcept {
        return viewportSize;
    }

    /**
     * @brief Projects a world position to screen coordinates based on the viewport size.
     * @param worldPosition The position in world coordinates to be projected.
     * @return A Vector2 representing the projected position in screen coordinates.
     */
    [[nodiscard]] Vector2 projectPosition(const Vector2& worldPosition) const noexcept {
        return {
            (worldPosition.x + 1.f) * 0.5f * viewportSize.x,
            (worldPosition.y + 1.f) * 0.5f * viewportSize.y
        };
    }

    /**
     * @brief Projects a world size to screen coordinates based on the viewport size.
     * @param worldSize The size in world coordinates to be projected.
     * @return A Vector2 representing the projected size in screen coordinates.
     */
    [[nodiscard]] Vector2 projectSize(const Vector2& worldSize) const noexcept {
        return {
            worldSize.x * 0.5f * viewportSize.x,
            worldSize.y * 0.5f * viewportSize.y
        };
    }
};

} // namespace core

#endif // BOMBERMAN_CORE_CAMERA_H