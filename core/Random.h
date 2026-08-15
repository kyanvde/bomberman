#ifndef BOMBERMAN_CORE_RANDOM_H
#define BOMBERMAN_CORE_RANDOM_H

#include <random>

namespace core {

/**
 * @brief The Random class provides a singleton implementation for generating random numbers.
 * It uses the Mersenne Twister algorithm (std::mt19937) for random number generation.
 */
class Random {
    /**
     * @brief Constructs a new Random object and initializes the random number generator.
     */
    Random() noexcept;

    /**
     * @brief The Mersenne Twister random number generator.
     */
    std::mt19937 gen;

public:
    /**
     * @brief Retrieves the singleton instance of the Random class.
     * @return A reference to the singleton Random instance.
     */
    [[nodiscard]] static Random& getInstance() noexcept;

    [[nodiscard]] double getRandomNumber(int min, int max);

    /**
     * @brief Rolls a random boolean outcome with the given probability of being true.
     * @param probability The probability (in [0, 1]) that this call returns true.
     * @return True with the given probability, false otherwise.
     */
    [[nodiscard]] bool chance(double probability);

    /**
     * @brief Reseeds the generator so subsequent draws repeat exactly. Intended for tests that need
     * a reproducible world -- a simulation that fails only on some runs is nearly impossible to
     * investigate. Normal play never calls this and keeps its std::random_device seeding.
     * @param seed The seed to restart the generator from.
     */
    void setSeed(unsigned int seed) noexcept;

    /**
     * @brief Deleted copy constructor to prevent copying of the Random instance.
     */
    Random(const Random&) = delete;

    /**
     * @brief Deleted copy assignment operator to prevent copying of the Random instance.
     */
    Random& operator=(const Random&) = delete;

    /**
     * @brief Deleted move constructor to prevent moving of the Random instance.
     */
    Random(Random&&) = delete;

    /**
     * @brief Deleted move assignment operator to prevent moving of the Random instance.
     */
    Random& operator=(Random&&) = delete;
};

} // namespace core

#endif // BOMBERMAN_CORE_RANDOM_H