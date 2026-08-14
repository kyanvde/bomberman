#ifndef BOMBERMAN_CORE_SCORE_H
#define BOMBERMAN_CORE_SCORE_H

#include "CharacterColor.h"
#include "GameEvent.h"
#include "Observer.h"

namespace core {

/**
 * @brief The Score class tracks the human-controlled Player's score for the current round. It is
 * an Observer, attached (by World::addEntity) to every entity in the world, and reacts to the
 * GameEvents entities fire when something scoring-relevant happens to them -- filtered to only
 * those attributed to the Player (White). Time alive is handled separately via an explicit
 * tick(deltaTime) call once per frame from GameState, rather than through the Observer channel,
 * to avoid double-counting from every entity's own routine per-frame notify.
 */
class Score final : public Observer {
    /**
     * @brief Points accumulated from discrete scoring events (blocks destroyed, power-ups
     * collected, enemies killed, and the win/lose bonus/penalty).
     */
    int eventPoints = 0;

    /**
     * @brief Total time, in seconds, this round has been alive so far.
     */
    float timeAlive = 0.f;

public:
    /**
     * @brief Reacts to a GameEvent fired by an observed entity. Events not attributed to the
     * Player are ignored.
     * @param event The event to react to.
     */
    void update(const GameEvent& event) override;

    /**
     * @brief Accumulates time alive. Called once per frame by GameState, independently of the
     * Observer channel.
     * @param deltaTime The time elapsed since the previous frame, in seconds.
     */
    void tick(float deltaTime);

    /**
     * @brief Retrieves the current total score: accumulated event points plus points earned for
     * time alive so far.
     * @return The current score.
     */
    [[nodiscard]] int getPoints() const noexcept;
};

} // namespace core

#endif // BOMBERMAN_CORE_SCORE_H
