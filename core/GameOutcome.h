#ifndef BOMBERMAN_CORE_GAMEOUTCOME_H
#define BOMBERMAN_CORE_GAMEOUTCOME_H

namespace core {

/**
 * @brief The current result of a round, as reported by World::getOutcome. The Player is the
 * human-controlled White character; every other character color is a bot.
 */
enum class GameOutcome {
    InProgress,
    PlayerWon,
    PlayerLost,
};

} // namespace core

#endif // BOMBERMAN_CORE_GAMEOUTCOME_H
