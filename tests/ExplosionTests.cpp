#include "CharacterColor.h"
#include "TestFactory.h"
#include "TestRunner.h"
#include "World.h"
#include "entities/Bomb.h"
#include "entities/Explosion.h"

#include <cstdio>
#include <fstream>
#include <memory>
#include <string>

namespace {

std::string writeDeterministicWorldFile(const std::string& path) {
    std::ofstream file(path);
    file << "WWW\n";
    file << "WWW\n";
    file << "WWW\n";
    return path;
}

// Builds a World from a small deterministic background, then removes every background entity,
// leaving a genuinely empty world with a known cellSize (mirrors DetonateBombTests.cpp's helper).
core::World makeEmptyWorld(const std::string& path) {
    writeDeterministicWorldFile(path);

    core::World world(std::make_shared<tests::TestFactory>(), path);
    std::remove(path.c_str());

    for (core::EntityId id = 0; id < 9; ++id) {
        world.markForRemoval(id);
    }
    world.update(0.f);

    return world;
}

} // namespace

void runExplosionTests(tests::TestRunner& runner) {
    using core::Bomb;
    using core::CharacterColor;
    using core::EntityId;
    using core::Explosion;
    using core::Vector2;
    using core::World;

    const Vector2 tileSize(0.2f, 0.2f);

    // --- An Explosion removes itself once its lifetime has elapsed. ---
    {
        World world = makeEmptyWorld("explosion_tests_tmp1.txt");

        auto explosion = std::make_unique<Explosion>(Vector2(0.f, 0.f), tileSize);
        const Explosion* explosionPtr = explosion.get();
        world.addEntity(std::move(explosion));
        const EntityId explosionId = explosionPtr->getId();

        runner.check(world.hasEntity(explosionId), "A freshly created explosion is present");

        world.update(0.1f);
        runner.check(world.hasEntity(explosionId), "The explosion is still present before its lifetime elapses");

        world.update(1.f); // well past its lifetime
        runner.check(!world.hasEntity(explosionId), "The explosion removes itself once its lifetime elapses");
    }

    // --- Detonating a bomb actually creates an Explosion entity, not just side effects. ---
    {
        World world = makeEmptyWorld("explosion_tests_tmp2.txt");

        auto bomb = std::make_unique<Bomb>(Vector2(0.f, 0.f), tileSize, CharacterColor::White, 1);
        const Bomb* bombPtr = bomb.get();
        world.addEntity(std::move(bomb));
        const EntityId bombId = bombPtr->getId();

        // The bomb is the last entity added before detonation, so the very next id assigned goes
        // to the first entity detonateBomb creates -- the explosion on the bomb's own tile.
        const EntityId expectedExplosionId = bombId + 1;

        world.detonateBomb(bombId, 1, CharacterColor::White);
        world.update(0.f); // flush the deferred bomb removal

        runner.check(world.hasEntity(expectedExplosionId),
                     "detonateBomb creates an Explosion entity on the blast tile");
    }

    // --- getLifetimeFraction() reports progress from 0 (just spawned) upward, driving
    //     ExplosionView's grow/fade animation. Only checked while the explosion is still present
    //     -- once it expires and is removed, the object itself is gone. ---
    {
        World world = makeEmptyWorld("explosion_tests_tmp3.txt");

        auto explosion = std::make_unique<Explosion>(Vector2(0.f, 0.f), tileSize);
        const Explosion* explosionPtr = explosion.get();
        world.addEntity(std::move(explosion));

        runner.checkNear(explosionPtr->getLifetimeFraction(), 0.f, "A freshly created explosion has fraction 0");

        world.update(0.1f);
        const float firstFraction = explosionPtr->getLifetimeFraction();
        runner.check(firstFraction > 0.f && firstFraction < 1.f,
                     "Partway through its lifetime, the fraction is between 0 and 1");

        world.update(0.1f);
        const float secondFraction = explosionPtr->getLifetimeFraction();
        runner.check(secondFraction > firstFraction, "The fraction keeps increasing as more time passes");
    }
}
