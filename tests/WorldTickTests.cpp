#include "TestEntities.h"
#include "TestFactory.h"
#include "TestRunner.h"
#include "World.h"

#include <cstdio>
#include <fstream>
#include <memory>
#include <string>
#include <vector>

namespace {

std::string writeDeterministicWorldFile() {
    const std::string path = "world_tick_tests_tmp.txt";
    std::ofstream file(path);
    file << "WWW\n";
    file << "WWW\n";
    file << "WWW\n";
    return path;
}

} // namespace

void runWorldTickTests(tests::TestRunner& runner) {
    using core::Vector2;
    using core::World;
    using tests::CountingEntity;

    const std::string path = writeDeterministicWorldFile();
    World world(std::make_shared<tests::TestFactory>(), path);
    std::remove(path.c_str());

    auto counter = std::make_unique<CountingEntity>(Vector2(0.f, 0.f), Vector2(0.1f, 0.1f));
    CountingEntity* counterPtr = counter.get();
    world.addEntity(std::move(counter));

    world.update(0.016f);
    runner.check(counterPtr->tickCount == 1, "onTick is called once per update()");
    runner.checkNear(counterPtr->lastDeltaTime, 0.016f, "onTick receives the deltaTime passed to update()");

    world.update(0.02f);
    runner.check(counterPtr->tickCount == 2, "onTick is called again on the following update()");
    runner.checkNear(counterPtr->lastDeltaTime, 0.02f, "onTick receives the latest deltaTime on each call");

    // Stress the iteration-invalidation fix: an entity that spawns many new entities mid-tick
    // (as an exploding bomb will in a later phase) must not corrupt update()'s own in-progress
    // iteration, crash, or cause reallocation-related undefined behavior.
    std::vector<CountingEntity*> spawned;
    auto spawner = std::make_unique<CountingEntity>(Vector2(0.f, 0.f), Vector2(0.1f, 0.1f));
    spawner->spawnCountOnFirstTick = 50;
    spawner->spawnedInto = &spawned;
    CountingEntity* spawnerPtr = spawner.get();
    world.addEntity(std::move(spawner));

    world.update(0.016f);
    runner.check(spawnerPtr->tickCount == 1, "The spawning entity itself still ticked exactly once");
    runner.check(spawned.size() == 50, "The spawning entity created all 50 children during its own onTick");

    bool noneTickedYet = true;
    for (const CountingEntity* child : spawned) {
        if (child->tickCount != 0) {
            noneTickedYet = false;
        }
    }
    runner.check(noneTickedYet, "Entities spawned mid-tick are not ticked within that same update() call");

    world.update(0.016f);
    bool allTickedNow = true;
    for (const CountingEntity* child : spawned) {
        if (child->tickCount != 1) {
            allTickedNow = false;
        }
    }
    runner.check(allTickedNow, "Entities spawned mid-tick are ticked on the following update() call");
    runner.check(spawnerPtr->tickCount == 2, "update() continues to behave correctly after a prior mid-tick spawn");
}
