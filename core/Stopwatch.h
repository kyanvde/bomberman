#ifndef BOMBERMAN_CORE_STOPWATCH_H
#define BOMBERMAN_CORE_STOPWATCH_H

#include <chrono>

namespace core {

/**
 * @brief The Stopwatch class provides a singleton implementation for measuring elapsed time.
 * It uses the high-resolution clock from the <chrono> library to track time intervals.
 */
class Stopwatch final {
public:
    /**
     * @brief Retrieves the singleton instance of the Stopwatch class.
     * @return A reference to the singleton Stopwatch instance.
     */
    [[nodiscard]] static Stopwatch& getInstance() noexcept;

    /**
     * @brief Updates the stopwatch by calculating the time elapsed since the last tick.
     * This method should be called periodically to update the delta time.
     */
    void tick() noexcept;

    /**
     * @brief Retrieves the time elapsed since the last tick in seconds.
     * @return The delta time in seconds as a float.
     */
    [[nodiscard]] float getDeltaTime() const noexcept;

    /**
     * @brief Deleted copy constructor to prevent copying of the Stopwatch instance.
     */
    Stopwatch(const Stopwatch&) = delete;

    /**
     * @brief Deleted copy assignment operator to prevent copying of the Stopwatch instance.
     */
    Stopwatch& operator=(const Stopwatch&) = delete;

    /**
     * @brief Deleted move constructor to prevent moving of the Stopwatch instance.
     */
    Stopwatch(Stopwatch&&) = delete;

    /**
     * @brief Deleted move assignment operator to prevent moving of the Stopwatch instance.
     */
    Stopwatch& operator=(Stopwatch&&) = delete;

private:
    /**
     * @brief Constructs a new Stopwatch object and initializes the last time point.
     */
    Stopwatch() noexcept = default;

    /**
     * @brief The last recorded time point used to calculate delta time.
     */
    std::chrono::high_resolution_clock::time_point lastTime{std::chrono::high_resolution_clock::now()};

    /**
     * @brief The time elapsed since the last tick in seconds.
     */
    float deltaTime{0.0f};
};

} // namespace core

#endif // BOMBERMAN_CORE_STOPWATCH_H