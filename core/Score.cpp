#include "Score.h"

namespace core {
namespace {
// Point values are suggested defaults (the assignment only specifies the scoring factors, not
// amounts), kept as small named constants near their point of use so they're easy to retune.
constexpr float pointsPerSecondAlive = 2.f;
constexpr int pointsPerBlockDestroyed = 10;
constexpr int pointsPerPowerUpCollected = 20;
constexpr int pointsPerEnemyKilled = 100;
constexpr int winBonus = 500;
constexpr int losePenalty = 100;
} // namespace

void Score::update(const GameEvent& event) {
    if (event.actor != CharacterColor::White) {
        return;
    }

    switch (event.type) {
    case GameEventType::Tick:
        break; // time alive is handled by tick(), not this routine per-frame notification
    case GameEventType::BlockDestroyed:
        eventPoints += pointsPerBlockDestroyed;
        break;
    case GameEventType::PowerUpCollected:
        eventPoints += pointsPerPowerUpCollected;
        break;
    case GameEventType::EntityKilled:
        eventPoints += pointsPerEnemyKilled;
        break;
    case GameEventType::GameWon:
        eventPoints += winBonus;
        break;
    case GameEventType::GameLost:
        eventPoints -= losePenalty;
        break;
    }
}

void Score::tick(const float deltaTime) {
    if (deltaTime > 0.f) {
        timeAlive += deltaTime;
    }
}

int Score::getPoints() const noexcept {
    const int total = eventPoints + static_cast<int>(timeAlive * pointsPerSecondAlive);
    return total > 0 ? total : 0;
}

} // namespace core
