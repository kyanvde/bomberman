#ifndef BOMBERMAN_CORE_HIGHSCORES_H
#define BOMBERMAN_CORE_HIGHSCORES_H

#include <string>
#include <vector>

namespace core {

/**
 * @brief Loads, updates, and persists the top-5 high score list to a plain-text file (one integer
 * score per line, highest first). A missing or corrupt file is treated as an empty list rather
 * than propagating an error -- a concrete example of the assignment's exception-handling
 * requirement applied to file I/O that's expected to fail on a fresh checkout.
 */
class HighScores {
    std::string path;
    std::vector<int> scores;

    /**
     * @brief Re-sorts (descending) and truncates the in-memory list to the top 5 entries.
     */
    void sortAndTrim();

public:
    /**
     * @brief Loads the high score list from the given file, if it exists and is readable.
     * @param path The path to the high score file.
     */
    explicit HighScores(std::string path);

    /**
     * @brief Retrieves the current top-5 list, highest first.
     * @return The current scores.
     */
    [[nodiscard]] const std::vector<int>& getScores() const noexcept { return scores; }

    /**
     * @brief Inserts a new score, re-sorts and truncates to the top 5, and re-saves the file.
     * @param score The score to record.
     */
    void record(int score);
};

} // namespace core

#endif // BOMBERMAN_CORE_HIGHSCORES_H
