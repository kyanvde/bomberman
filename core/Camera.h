#ifndef BOMBERMAN_CORE_CAMERA_H
#define BOMBERMAN_CORE_CAMERA_H

#include "Vector2.h"

namespace core {

/**
 * @brief The Camera class represents a camera in the game world.
 * It provides methods to project world coordinates to screen coordinates based on the viewport size.
 *
 * The world is always a square [-1, 1] x [-1, 1] region, but the viewport it's projected into
 * rarely has a matching aspect ratio (the window can be resized to anything). Scaling each axis
 * independently to fill the viewport would stretch every sprite by a different amount depending on
 * the window's current width-to-height ratio -- exactly the resolution-dependent look the
 * assignment's normalized coordinate system is meant to prevent. Instead, both axes are scaled by
 * the same factor (the viewport's smaller dimension), and the result is centered in the other axis,
 * leaving even letterbox/pillarbox bars rather than distorting anything.
 */
class Camera {
    /**
     * @brief The size of the viewport in screen coordinates.
     */
    Vector2 viewportSize;

    /**
     * @brief The single scale factor applied to both axes, and the offset that centers the
     * projected [-1, 1] square within the viewport's larger dimension.
     */
    struct Projection {
        float scale;
        float offsetX;
        float offsetY;
    };

    /**
     * @brief Computes the current uniform scale factor and centering offsets from viewportSize.
     */
    [[nodiscard]] Projection projection() const noexcept;

public:
    /**
     * @brief Constructs a new Camera object with the specified viewport size.
     * @param viewportSize The size of the viewport in screen coordinates (default is (0, 0)).
     */
    explicit Camera(const Vector2& viewportSize = Vector2(0.f, 0.f));

    /**
     * @brief Sets the size of the viewport.
     * @param newViewportSize The new size of the viewport in screen coordinates.
     */
    void setViewportSize(const Vector2& newViewportSize);

    /**
     * @brief Retrieves the size of the viewport.
     * @return A constant reference to the Vector2 representing the viewport size.
     */
    [[nodiscard]] const Vector2& getViewportSize() const noexcept;

    /**
     * @brief Projects a world position to screen coordinates based on the viewport size.
     * @param worldPosition The position in world coordinates to be projected.
     * @return A Vector2 representing the projected position in screen coordinates.
     */
    [[nodiscard]] Vector2 projectPosition(const Vector2& worldPosition) const noexcept;

    /**
     * @brief Projects a world size to screen coordinates based on the viewport size.
     * @param worldSize The size in world coordinates to be projected.
     * @return A Vector2 representing the projected size in screen coordinates.
     */
    [[nodiscard]] Vector2 projectSize(const Vector2& worldSize) const noexcept;
};

} // namespace core

#endif // BOMBERMAN_CORE_CAMERA_H