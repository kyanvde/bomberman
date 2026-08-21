#include "PowerUp.h"

namespace core {
PowerUp::PowerUp(const Vector2& pos, const Vector2& size, const PowerUpType type)
    : EntityModel(pos, size), type(type) {}

PowerUpType PowerUp::getType() const noexcept { return type; }

bool PowerUp::isPowerUp() const noexcept { return true; }

std::optional<PowerUpType> PowerUp::getPowerUpType() const noexcept { return type; }
} // namespace core