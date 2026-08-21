#include "Grass.h"

namespace core {
Grass::Grass(const Vector2& pos, const Vector2& size) : EntityModel(pos, size) {}

bool Grass::isGrass() const noexcept { return true; }
} // namespace core