#include "BasicAIController.h"
#include "CharacterColor.h"
#include "TestFactory.h"
#include "TestRunner.h"
#include "World.h"
#include "entities/Bomb.h"
#include "entities/Character.h"
#include "entities/PowerUp.h"
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

void runAIControllerTests(tests::TestRunner& runner) {
    using core::BasicAIController;
    using core::Bomb;
    using core::Character;
    using core::CharacterColor;
    using core::Decision;
    using core::PowerUp;
    using core::PowerUpType;
    using core::Vector2;
    using core::Wall;
    using core::World;

    const Vector2 tileSize(0.2f, 0.2f);
    const BasicAIController controller(4.f, 6.f);

    // --- Flees imminent bomb danger, stepping to the first neighboring tile that is both open
    //     and out of every threatening bomb's blast range. ---
    {
        World world = makeEmptyWorld("ai_tests_tmp1.txt");
        auto bomb = std::make_unique<Bomb>(Vector2(-0.2f, 0.f), tileSize, CharacterColor::White, 1);
        world.addEntity(std::move(bomb));

        const Character self(Vector2(0.f, 0.f), tileSize, CharacterColor::Blue); // one tile east of the bomb

        const Decision decision = controller.decide(world, self);
        runner.check(decision.direction.x == 1.f && decision.direction.y == 0.f,
                     "Fleeing danger steps toward the first safe, open neighboring tile");
        runner.check(!decision.placeBomb, "Fleeing danger does not also place a bomb");
    }

    // --- Bombs an adjacent destructible wall and immediately retreats from it. ---
    {
        World world = makeEmptyWorld("ai_tests_tmp2.txt");
        auto wall = std::make_unique<Wall>(Vector2(0.2f, 0.f), tileSize, true); // one tile east
        world.addEntity(std::move(wall));

        const Character self(Vector2(0.f, 0.f), tileSize, CharacterColor::Blue);

        const Decision decision = controller.decide(world, self);
        runner.check(decision.placeBomb, "A character next to a destructible wall places a bomb");
        // West, north and south are all equally good here, so which one it picks is not meaningful;
        // what matters is that it leaves, and does not head into the wall it has just bombed.
        runner.check(decision.direction.x <= 0.f, "After bombing a wall, the character does not head into it");
        runner.check(decision.direction.x != 0.f || decision.direction.y != 0.f,
                     "After bombing a wall, the character retreats rather than standing on the bomb");
    }

    // --- Seeks a power-up within its search radius when nothing more urgent is happening. ---
    {
        World world = makeEmptyWorld("ai_tests_tmp3.txt");
        auto powerUp = std::make_unique<PowerUp>(Vector2(0.6f, 0.f), tileSize, PowerUpType::Fire); // 3 tiles east
        world.addEntity(std::move(powerUp));

        const Character self(Vector2(0.f, 0.f), tileSize, CharacterColor::Blue);

        const Decision decision = controller.decide(world, self);
        runner.check(decision.direction.x == 1.f && decision.direction.y == 0.f,
                     "A character with nothing more urgent moves toward a nearby power-up");
        runner.check(!decision.placeBomb, "Seeking a power-up does not place a bomb");
    }

    // --- Attacks a nearby enemy: approaches from a distance, bombs once adjacent. ---
    {
        World world = makeEmptyWorld("ai_tests_tmp4.txt");
        auto enemy = std::make_unique<Character>(Vector2(0.2f, 0.f), tileSize, CharacterColor::Red); // adjacent
        world.addEntity(std::move(enemy));

        const Character self(Vector2(0.f, 0.f), tileSize, CharacterColor::Blue);

        const Decision decision = controller.decide(world, self);
        runner.check(decision.direction.x == 1.f && decision.direction.y == 0.f,
                     "A character next to an enemy moves toward it");
        runner.check(decision.placeBomb, "A character adjacent to an enemy places a bomb to attack");
    }

    // --- Danger always takes priority over every other behavior, and only a genuinely safe
    //     neighboring tile is chosen even when a nearer-looking one is tempting. ---
    {
        World world = makeEmptyWorld("ai_tests_tmp5.txt");
        // A radius-2 bomb one tile west of self reaches two tiles east too, so the immediate east
        // neighbor -- right where a power-up happens to sit -- is still inside the blast.
        auto bomb = std::make_unique<Bomb>(Vector2(-0.2f, 0.f), tileSize, CharacterColor::White, 2);
        auto powerUp = std::make_unique<PowerUp>(Vector2(0.2f, 0.f), tileSize, PowerUpType::Fire);
        world.addEntity(std::move(bomb));
        world.addEntity(std::move(powerUp));

        const Character self(Vector2(0.f, 0.f), tileSize, CharacterColor::Blue);

        const Decision decision = controller.decide(world, self);
        runner.check(!(decision.direction.x == 1.f && decision.direction.y == 0.f),
                     "Danger response does not walk toward a tile still within blast range, even with a "
                     "power-up sitting there");
        // North and south both lead out of this blast; either is a correct escape.
        runner.check(decision.direction.x == 0.f && decision.direction.y != 0.f,
                     "Danger response leaves along the axis the blast does not cover");
    }
}
