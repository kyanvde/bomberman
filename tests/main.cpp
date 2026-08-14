#include "TestRunner.h"

void runVector2Tests(tests::TestRunner& runner);
void runCameraTests(tests::TestRunner& runner);
void runRandomTests(tests::TestRunner& runner);
void runStopwatchTests(tests::TestRunner& runner);
void runCharacterTests(tests::TestRunner& runner);
void runWallTests(tests::TestRunner& runner);
void runWorldTests(tests::TestRunner& runner);
void runWorldTickTests(tests::TestRunner& runner);
void runBombTests(tests::TestRunner& runner);
void runDetonateBombTests(tests::TestRunner& runner);

/**
 * @brief Entry point for the core_tests executable.
 * Runs every registered test suite against the core (SFML-free) game-logic
 * library and returns a non-zero exit code if any check failed, so CTest
 * (and CI) can detect regressions.
 */
int main() {
    tests::TestRunner runner;

    runVector2Tests(runner);
    runCameraTests(runner);
    runRandomTests(runner);
    runStopwatchTests(runner);
    runCharacterTests(runner);
    runWallTests(runner);
    runWorldTests(runner);
    runWorldTickTests(runner);
    runBombTests(runner);
    runDetonateBombTests(runner);

    return runner.summarize();
}
