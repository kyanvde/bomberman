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

bool Random::chance(const double probability) {
    std::uniform_real_distribution<> dis(0.0, 1.0);
    return dis(gen) < probability;
}

void Random::setSeed(const unsigned int seed) noexcept { gen.seed(seed); }

Random::Random() noexcept : gen(std::random_device{}()) {}

} // namespace core