#include "Stopwatch.h"
#include "TestRunner.h"

#include <chrono>
#include <thread>

void runStopwatchTests(tests::TestRunner& runner) {
    core::Stopwatch::getInstance().tick();
    runner.check(core::Stopwatch::getInstance().getDeltaTime() >= 0.f, "Stopwatch delta time is never negative");

    std::this_thread::sleep_for(std::chrono::milliseconds(20));
    core::Stopwatch::getInstance().tick();
    const float delta = core::Stopwatch::getInstance().getDeltaTime();

    runner.check(delta >= 0.015f, "Stopwatch delta time reflects at least ~15ms after a 20ms sleep");
    runner.check(delta <= 2.f, "Stopwatch delta time stays within a sane upper bound");
}
