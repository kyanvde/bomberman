#ifndef BOMBERMAN_CORE_AICONTROLLER_H
#define BOMBERMAN_CORE_AICONTROLLER_H

#include "Vector2.h"

namespace core {

class EntityModel;
class World;

/**
 * @brief What an AIController wants to do this tick: move in a direction (a zero vector means
 * stay in place) and, independently, whether to place a bomb at the current tile.
 */
struct Decision {
    Vector2 direction;
    bool placeBomb = false;
};

/**
 * @brief The AIController class is the strategy interface for computer-controlled characters. An
 * implementation observes the World read-only and returns a Decision; it never mutates the World
 * directly -- Character::onTick applies the decision through the same World::moveCharacter and
 * World::placeBomb methods the human player's input uses, so there is no duplicated
 * movement/bombing logic between player and bots.
 */
class AIController {
public:
    virtual ~AIController() = default;

    /**
     * @brief Decides what the controlled character should do this tick.
     * @param world The world to observe (read-only).
     * @param self The controlled character's own entity, for its position, size, color, and stats.
     * @return The chosen Decision.
     */
    [[nodiscard]] virtual Decision decide(const World& world, const EntityModel& self) const = 0;
};

} // namespace core

#endif // BOMBERMAN_CORE_AICONTROLLER_H
