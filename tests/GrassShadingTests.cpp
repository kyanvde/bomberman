#include "CharacterColor.h"
#include "TestFactory.h"
#include "TestRunner.h"
#include "World.h"
#include "entities/Bomb.h"
#include "entities/Wall.h"

#include <cmath>
#include <cstdio>
#include <fstream>
#include <memory>
#include <string>

namespace {

core::World makeEmptyWorld(const std::string& path, const std::shared_ptr<tests::TestFactory>& factory) {
    {
        std::ofstream file(path);
        file << "WWW\n";
        file << "WWW\n";
        file << "WWW\n";
    }

    core::World world(factory, path);
    std::remove(path.c_str());

    for (core::EntityId id = 0; id < 9; ++id) {
        world.markForRemoval(id);
    }
    world.update(0.f);

    return world;
}

bool samePosition(const core::Vector2& a, const core::Vector2& b) {
    constexpr float epsilon = 0.0001f;
    return std::abs(a.x - b.x) < epsilon && std::abs(a.y - b.y) < epsilon;
}

} // namespace

void runGrassShadingTests(tests::TestRunner& runner) {
    using core::Bomb;
    using core::CharacterColor;
    using core::EntityId;
    using core::Vector2;
    using core::Wall;
    using core::World;

    const Vector2 tileSize(0.2f, 0.2f);

    // --- A grass tile shaded because of a wall above it must stop looking shaded once that wall
    //     is destroyed. Shading is decided once, when the grass is created, from a snapshot of
    //     what's above it at that moment -- so unless the tile below a destroyed wall is
    //     specifically revisited, it keeps showing a shadow cast by a wall that's now gone. ---
    {
        auto factory = std::make_shared<tests::TestFactory>();
        World world = makeEmptyWorld("grass_shading_tests_tmp1.txt", factory);

        const Vector2 wallTile(0.f, 0.f);
        const Vector2 belowTile(0.f, 0.2f); // directly south of the wall
        const Vector2 bombTile(-0.2f, 0.f); // one tile west of the wall

        // Set up the precondition exactly as WorldLoader would have originally: grass below the
        // wall, shaded, because the wall above it existed at the time it was created.
        world.addEntity(factory->createGrass(belowTile, tileSize, true));
        world.addEntity(std::make_unique<Wall>(wallTile, tileSize, true));

        auto bomb = std::make_unique<Bomb>(bombTile, tileSize, CharacterColor::White, 1);
        const Bomb* bombPtr = bomb.get();
        world.addEntity(std::move(bomb));

        const EntityId bombId = bombPtr->getId();

        world.detonateBomb(bombId, 1, CharacterColor::White);
        world.update(0.f);

        runner.check(world.hasGrassAt(belowTile, tileSize),
                     "Grass still exists below the tile once the wall above it is destroyed");

        const tests::TestFactory::GrassCreation* lastCreationBelow = nullptr;
        for (const tests::TestFactory::GrassCreation& creation : factory->grassCreations) {
            if (samePosition(creation.position, belowTile)) {
                lastCreationBelow = &creation;
            }
        }

        runner.check(lastCreationBelow != nullptr,
                     "The grass below the destroyed wall was recreated, not left untouched");
        runner.check(lastCreationBelow != nullptr && !lastCreationBelow->shaded,
                     "The grass below a destroyed wall is unshaded once the wall is gone");
    }

    // --- A grass tile with nothing above it (no wall was ever there) is unaffected by an
    //     unrelated wall being destroyed elsewhere. ---
    {
        auto factory = std::make_shared<tests::TestFactory>();
        World world = makeEmptyWorld("grass_shading_tests_tmp2.txt", factory);

        const Vector2 wallTile(0.f, 0.f);
        const Vector2 farGrassTile(0.6f, 0.6f); // nowhere near the wall
        const Vector2 bombTile(-0.2f, 0.f);

        world.addEntity(factory->createGrass(farGrassTile, tileSize, false));
        world.addEntity(std::make_unique<Wall>(wallTile, tileSize, true));

        auto bomb = std::make_unique<Bomb>(bombTile, tileSize, CharacterColor::White, 1);
        const Bomb* bombPtr = bomb.get();
        world.addEntity(std::move(bomb));

        world.detonateBomb(bombPtr->getId(), 1, CharacterColor::White);
        world.update(0.f);

        int creationsAtFarTile = 0;
        for (const tests::TestFactory::GrassCreation& creation : factory->grassCreations) {
            if (samePosition(creation.position, farGrassTile)) {
                ++creationsAtFarTile;
            }
        }
        runner.check(creationsAtFarTile == 1,
                     "An unrelated grass tile is not recreated by a distant wall's destruction");
    }
}
