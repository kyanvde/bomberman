#ifndef BOMBERMAN_CORE_GAMEEVENT_H
#define BOMBERMAN_CORE_GAMEEVENT_H

#include "CharacterColor.h"

namespace core {

/**
 * @brief The kind of notable, scoring-relevant thing that just happened to an entity, carried
 * alongside the routine Subject::notify() every entity already sends its observers whenever its
 * position or animation changes. Tick is that routine case; every other value is fired
 * deliberately by World at the moment it happens.
 */
enum class GameEventType {
    Tick,
    BlockDestroyed,
    PowerUpCollected,
    EntityKilled,
    GameWon,
    GameLost,
};

/**
 * @brief A notification payload: what happened, and which character it should be attributed to
 * (the bomb owner for BlockDestroyed/EntityKilled, the picking-up character for
 * PowerUpCollected). Defaults to a routine Tick from the Player, so every existing
 * Subject::notify()/Observer::update() call site keeps compiling unchanged.
 */
struct GameEvent {
    GameEventType type = GameEventType::Tick;
    CharacterColor actor = CharacterColor::White;
};

} // namespace core

#endif // BOMBERMAN_CORE_GAMEEVENT_H
