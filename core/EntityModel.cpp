#include "EntityModel.h"

namespace core {

EntityModel::EntityModel(const Vector2& pos, const Vector2& size) : position(pos), size(size) {}

EntityModel::~EntityModel() = default;

void EntityModel::setAnimationType(const AnimationType newAnimationType) {
    this->animationType = newAnimationType;
    notify();
}

const Vector2& EntityModel::getPosition() const noexcept { return position; }

EntityId EntityModel::getId() const noexcept { return id; }

void EntityModel::setId(const EntityId newId) noexcept { id = newId; }

const Vector2& EntityModel::getSize() const noexcept { return size; }

void EntityModel::setPosition(const Vector2& newPosition) {
    position = newPosition;
    notify();
}

bool EntityModel::isPlayerControlled() const noexcept { return false; }

std::optional<CharacterColor> EntityModel::getCharacterColor() const noexcept { return std::nullopt; }

bool EntityModel::isCharacterOfColor(const CharacterColor& color) const noexcept {
    return getCharacterColor() == color;
}

bool EntityModel::canPlaceBomb() const noexcept { return false; }

int EntityModel::getBombRadius() const noexcept { return 1; }

void EntityModel::onBombPlaced() {}

void EntityModel::onBombExploded() {}

int EntityModel::renderLayer() const noexcept { return 0; }

bool EntityModel::blocksMovementOf(const EntityModel& mover, const Vector2& moverPosition,
                                   const Vector2& moverSize) const {
    return mover.isBlockedBy(*this, moverPosition, moverSize);
}

bool EntityModel::isBlockedBy(const EntityModel&, const Vector2&, const Vector2&) const { return false; }

bool EntityModel::blocksCharacterMovement(const Character&, const Vector2&, const Vector2&) const { return false; }

void EntityModel::onMovementAttempt(const Vector2&) {}

AnimationType EntityModel::getAnimationType() const { return animationType; }

void EntityModel::onTick(World&, EntityId, float) {}

bool EntityModel::blocksExplosion() const noexcept { return false; }

bool EntityModel::isDestructibleByExplosion() const noexcept { return false; }

bool EntityModel::isKilledByExplosion() const noexcept { return false; }

void EntityModel::onExplosionKill() {}

bool EntityModel::isAlive() const noexcept { return true; }

bool EntityModel::isBomb() const noexcept { return false; }

bool EntityModel::threatensTile(const Vector2&, const Vector2&) const noexcept { return false; }

float EntityModel::getLifetimeFraction() const noexcept { return 0.f; }

bool EntityModel::isPowerUp() const noexcept { return false; }

bool EntityModel::isGrass() const noexcept { return false; }

std::optional<PowerUpType> EntityModel::getPowerUpType() const noexcept { return std::nullopt; }

void EntityModel::applyPowerUp(PowerUpType) {}

float EntityModel::getSpeedMultiplier() const noexcept { return 1.f; }

void EntityModel::detonate(World&, EntityId) {}

} // namespace core
