#ifndef BOMBERMAN_CORE_VECTOR2_H
#define BOMBERMAN_CORE_VECTOR2_H

namespace core {

/**
 * @brief The Vector2 struct represents a 2D vector with x and y components.
 * It provides basic vector operations such as addition, subtraction, and scalar multiplication.
 */
struct Vector2 {
    /**
     * @brief The x component of the vector.
     */
    float x;

    /**
     * @brief The y component of the vector.
     */
    float y;

    /**
     * @brief Constructs a new Vector2 object with default values (0, 0).
     */
    Vector2() : x(0), y(0) {}

    /**
     * @brief Constructs a new Vector2 object with the specified x and y components.
     * @param x The x component of the vector.
     * @param y The y component of the vector.
     */
    Vector2(const float x, const float y) : x(x), y(y) {}

    /**
     * @brief Adds two Vector2 objects together and returns the result.
     * @param vector2 The other Vector2 object to add.
     * @return A new Vector2 object representing the sum of the two vectors.
     */
    Vector2 operator+(const Vector2& vector2) const {
        return {x + vector2.x, y + vector2.y};
    }

    /**
     * @brief Subtracts one Vector2 object from another and returns the result.
     * @param vector2 The other Vector2 object to subtract.
     * @return A new Vector2 object representing the difference of the two vectors.
     */
    Vector2 operator-(const Vector2& vector2) const {
        return {x - vector2.x, y - vector2.y};
    }

    /**
     * @brief Multiplies the Vector2 object by a scalar value and returns the result.
     * @param i The scalar value to multiply by.
     * @return A new Vector2 object representing the scaled vector.
     */
    Vector2 operator*(const int i) const {
        return {x*static_cast<float>(i), y*static_cast<float>(i)};
    }
};

} // namespace core

#endif // BOMBERMAN_CORE_VECTOR2_H