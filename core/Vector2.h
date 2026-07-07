#ifndef BOMBERMAN_CORE_VECTOR2_H
#define BOMBERMAN_CORE_VECTOR2_H

namespace core {

struct Vector2 {
    float x;
    float y;

    Vector2() : x(0), y(0) {}

    Vector2(const float x, const float y) : x(x), y(y) {}

    Vector2 operator+(const Vector2& vector2) const {
        return {x + vector2.x, y + vector2.y};
    }

    Vector2 operator-(const Vector2& vector2) const {
        return {x - vector2.x, y - vector2.y};
    }

    Vector2 operator*(int i) const {
        return {x*i, y*i};
    }
};

} // namespace core

#endif // BOMBERMAN_CORE_VECTOR2_H