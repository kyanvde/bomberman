#include "Random.h"

namespace core {

Random& Random::getInstance() noexcept {
    static Random instance;
    return instance;
}

double Random::getRandomNumber(const int min, const int max) {
    std::uniform_real_distribution<> dis(min, max);
    return dis(gen);
}

Random::Random() noexcept {
    std::random_device rd;
    gen = std::mt19937(rd());
}

} // namespace core