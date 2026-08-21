#include "Vector2.h"

namespace core {

Vector2::Vector2() : x(0), y(0) {}

Vector2::Vector2(const float x, const float y) : x(x), y(y) {}

Vector2 Vector2::operator+(const Vector2& vector2) const { return {x + vector2.x, y + vector2.y}; }

Vector2 Vector2::operator-(const Vector2& vector2) const { return {x - vector2.x, y - vector2.y}; }

Vector2 Vector2::operator*(const int i) const { return {x * static_cast<float>(i), y * static_cast<float>(i)}; }

} // namespace core
