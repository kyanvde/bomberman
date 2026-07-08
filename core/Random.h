#ifndef BOMBERMAN_CORE_RANDOM_H
#define BOMBERMAN_CORE_RANDOM_H

#include <random>

namespace core {

class Random {
public:
    [[nodiscard]] static Random& getInstance() noexcept;

    [[nodiscard]] double getRandomNumber(int min, int max);

    Random(const Random&) = delete;

    Random& operator=(const Random&) = delete;

    Random(Random&&) = delete;

    Random& operator=(Random&&) = delete;

private:
    Random() noexcept;

    std::mt19937 gen;
};

} // namespace core

#endif // BOMBERMAN_CORE_RANDOM_H