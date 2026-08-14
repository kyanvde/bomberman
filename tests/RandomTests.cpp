#include "Random.h"
#include "TestRunner.h"

void runRandomTests(tests::TestRunner& runner) {
    bool allInRange = true;
    for (int i = 0; i < 1000; ++i) {
        const double value = core::Random::getInstance().getRandomNumber(0, 1);
        if (value < 0.0 || value > 1.0) {
            allInRange = false;
            break;
        }
    }
    runner.check(allInRange, "Random::getRandomNumber(0, 1) stays within [0, 1] over many samples");

    bool sawVariety = false;
    const double first = core::Random::getInstance().getRandomNumber(0, 1000000);
    for (int i = 0; i < 20; ++i) {
        if (core::Random::getInstance().getRandomNumber(0, 1000000) != first) {
            sawVariety = true;
            break;
        }
    }
    runner.check(sawVariety, "Random::getRandomNumber produces varying values across repeated calls");
}
