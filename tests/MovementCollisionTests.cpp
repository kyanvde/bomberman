#include "CharacterColor.h"
#include "TestFactory.h"
#include "TestRunner.h"
#include "World.h"
#include "entities/Character.h"
#include "entities/Grass.h"
#include "entities/Wall.h"

#include <cstdio>
#include <fstream>
#include <memory>
#include <string>

namespace {

// A 10x10 all-wall background gives a clean cellSize of (0.2, 0.2), then the background is
// stripped away, leaving a genuinely empty world a test can populate itself. Mirrors the
// makeEmptyWorld helper used throughout the other World-level test files.
core::World makeEmptyWorld(const std::string& path) {
    {
        std::ofstream file(path);
        for (int row = 0; row < 10; ++row) {
            file << "WWWWWWWWWW\n";
        }
    }

    core::World world(std::make_shared<tests::TestFactory>(), path);
    std::remove(path.c_str());

    for (core::EntityId id = 0; id < 100; ++id) {
        world.markForRemoval(id);
    }
    world.update(0.f);

    return world;
}

} // namespace

void runMovementCollisionTests(tests::TestRunner& runner) {
    using core::Character;
    using core::CharacterColor;
    using core::EntityId;
    using core::Grass;
    using core::Vector2;
    using core::Wall;
    using core::World;

    const Vector2 tileSize(0.2f, 0.2f);

    // --- A character is stopped by a wall directly in its path -- never enters the wall's tile,
    //     no matter how long it keeps trying to walk into it. ---
    {
        World world = makeEmptyWorld("movement_tests_tmp1.txt");

        auto character = std::make_unique<Character>(Vector2(0.f, 0.f), tileSize, CharacterColor::White);
        auto wall = std::make_unique<Wall>(Vector2(0.2f, 0.f), tileSize, false); // east neighbor, indestructible
        const Character* characterPtr = character.get();

        world.addEntity(std::move(character));
        world.addEntity(std::move(wall));

        const EntityId characterId = characterPtr->getId();
        const Vector2 startPosition = characterPtr->getPosition();

        for (int i = 0; i < 60; ++i) {
            world.moveCharacter(characterId, Vector2(1.f, 0.f), 0.05f);
        }

        runner.check(characterPtr->getPosition().x < startPosition.x + tileSize.x,
                     "A character is stopped before entering a wall's tile, however long it keeps trying");
    }

    // --- A character can move freely through grass, well past where a wall would have stopped it. ---
    {
        World world = makeEmptyWorld("movement_tests_tmp2.txt");

        auto character = std::make_unique<Character>(Vector2(0.f, 0.f), tileSize, CharacterColor::White);
        auto grass = std::make_unique<Grass>(Vector2(0.2f, 0.f), tileSize);
        const Character* characterPtr = character.get();

        world.addEntity(std::move(character));
        world.addEntity(std::move(grass));

        const EntityId characterId = characterPtr->getId();
        const Vector2 startPosition = characterPtr->getPosition();

        for (int i = 0; i < 10; ++i) {
            world.moveCharacter(characterId, Vector2(1.f, 0.f), 0.05f);
        }

        runner.check(characterPtr->getPosition().x > startPosition.x + tileSize.x * 0.5f,
                     "A character moves freely across a grass tile");
    }

    // --- A character is clamped at the world's edge rather than moving past it. ---
    {
        World world = makeEmptyWorld("movement_tests_tmp3.txt");

        auto character = std::make_unique<Character>(Vector2(-1.f, 0.f), tileSize, CharacterColor::White);
        const Character* characterPtr = character.get();
        world.addEntity(std::move(character));

        const EntityId characterId = characterPtr->getId();

        for (int i = 0; i < 60; ++i) {
            world.moveCharacter(characterId, Vector2(-1.f, 0.f), 0.05f);
        }

        runner.check(characterPtr->getPosition().x >= -1.f, "A character never moves past the world's western edge");
    }

    // --- Diagonal movement blocked on one axis still proceeds on the other. ---
    {
        World world = makeEmptyWorld("movement_tests_tmp4.txt");

        auto character = std::make_unique<Character>(Vector2(0.f, 0.f), tileSize, CharacterColor::White);
        auto wall = std::make_unique<Wall>(Vector2(0.2f, 0.f), tileSize, false); // blocks east only
        const Character* characterPtr = character.get();

        world.addEntity(std::move(character));
        world.addEntity(std::move(wall));

        const EntityId characterId = characterPtr->getId();
        const Vector2 startPosition = characterPtr->getPosition();

        for (int i = 0; i < 10; ++i) {
            world.moveCharacter(characterId, Vector2(1.f, 1.f), 0.05f); // diagonal: east blocked, south open
        }

        const Vector2 endPosition = characterPtr->getPosition();
        runner.check(endPosition.x < startPosition.x + tileSize.x, "Movement is blocked on the axis with a wall");
        runner.check(endPosition.y > startPosition.y, "Movement still proceeds on the unblocked axis");
    }
}
