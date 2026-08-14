#ifndef BOMBERMAN_TESTS_TESTRUNNER_H
#define BOMBERMAN_TESTS_TESTRUNNER_H

#include <cmath>
#include <iostream>
#include <string>

namespace tests {

/**
 * @brief Minimal, dependency-free test harness for core's game logic.
 * Records pass/fail counts and prints a summary; summarize() returns a
 * process exit code suitable for CTest (0 on success, 1 if anything failed).
 */
class TestRunner {
    int passed = 0;
    int failed = 0;

public:
    void check(bool condition, const std::string& description) {
        if (condition) {
            ++passed;
        } else {
            ++failed;
            std::cerr << "FAILED: " << description << '\n';
        }
    }

    void checkNear(float actual, float expected, const std::string& description, float epsilon = 0.0001f) {
        check(std::abs(actual - expected) <= epsilon,
              description + " (expected " + std::to_string(expected) + ", got " + std::to_string(actual) + ")");
    }

    [[nodiscard]] int summarize() const {
        std::cout << passed << " passed, " << failed << " failed\n";
        return failed == 0 ? 0 : 1;
    }
};

} // namespace tests

#endif // BOMBERMAN_TESTS_TESTRUNNER_H
