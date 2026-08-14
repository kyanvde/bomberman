#include "CharacterColor.h"
#include "TestFactory.h"
#include "TestRunner.h"
#include "World.h"
#include "entities/Bomb.h"
#include "entities/Wall.h"

#include <cstdio>
#include <fstream>
#include <memory>
#include <string>

namespace {

// Mirrors DetonateBombTests.cpp's makeEmptyWorld helper: builds a small deterministic background,
// then strips it away, leaving a genuinely empty world a test can populate itself.
core::World makeEmptyWorld(const std::string& path) {
    {
        std::ofstream file(path);
        file << "WWW\n";
        file << "WWW\n";
        file << "WWW\n";
    }

    core::World world(std::make_shared<tests::TestFactory>(), path);
    std::remove(path.c_str());

    for (core::EntityId id = 0; id < 9; ++id) {
        world.markForRemoval(id);
    }
    world.update(0.f);

    return world;
}

} // namespace

void runPowerUpSpawnTests(tests::TestRunner& runner) {
    using core::Bomb;
    using core::CharacterColor;
    using core::EntityId;
    using core::Vector2;
    using core::Wall;
    using core::World;

    // --- Destroying a destructible wall sometimes (not always, not never) leaves a power-up
    //     behind, alongside the grass tile it always leaves. Run many independent trials -- each
    //     an isolated bomb/wall pair far enough apart not to interact -- and check the outcome
    //     distribution isn't degenerate in either direction. ---
    World world = makeEmptyWorld("powerup_spawn_tests_tmp.txt");

    const Vector2 tileSize(0.02f, 0.02f);
    constexpr int trialCount = 40;

    int powerUpSpawnCount = 0;

    for (int i = 0; i < trialCount; ++i) {
        const float bombX = -0.99f + static_cast<float>(i) * 0.05f;
        const Vector2 bombTile(bombX, 0.f);
        const Vector2 wallTile(bombX + tileSize.x, 0.f);

        auto wall = std::make_unique<Wall>(wallTile, tileSize, true);
        auto bomb = std::make_unique<Bomb>(bombTile, tileSize, CharacterColor::White, 1);

        const Bomb* bombPtr = bomb.get();

        world.addEntity(std::move(wall));
        world.addEntity(std::move(bomb));

        const EntityId bombId = bombPtr->getId();

        world.detonateBomb(bombId, 1, CharacterColor::White);
        world.update(0.f);

        runner.check(world.hasGrassAt(wallTile, tileSize),
                     "A destroyed destructible wall always leaves grass behind, power-up or not");

        if (world.hasPowerUpAt(wallTile, tileSize)) {
            ++powerUpSpawnCount;
        }
    }

    runner.check(powerUpSpawnCount > 0, "At least one destroyed wall (out of many trials) leaves a power-up behind");
    runner.check(powerUpSpawnCount < trialCount,
                 "Not every destroyed wall (out of many trials) leaves a power-up behind");
}
