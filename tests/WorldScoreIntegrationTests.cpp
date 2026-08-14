#include "CharacterColor.h"
#include "PowerUpType.h"
#include "Score.h"
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
// stripped away, leaving a genuinely empty world a test can populate itself -- with a Score
// attached from construction, so entities added afterward are covered by World::addEntity's
// auto-attach, matching how a real GameState-constructed World is set up.
core::World makeEmptyWorldWithScore(const std::string& path, const std::shared_ptr<core::Score>& score) {
    {
        std::ofstream file(path);
        for (int row = 0; row < 10; ++row) {
            file << "WWWWWWWWWW\n";
        }
    }

    core::World world(std::make_shared<tests::TestFactory>(), path, score);
    std::remove(path.c_str());

    for (core::EntityId id = 0; id < 100; ++id) {
        world.markForRemoval(id);
    }
    world.update(0.f);

    return world;
}

} // namespace

void runWorldScoreIntegrationTests(tests::TestRunner& runner) {
    using core::Bomb;
    using core::Character;
    using core::CharacterColor;
    using core::EntityId;
    using core::PowerUp;
    using core::PowerUpType;
    using core::Score;
    using core::Vector2;
    using core::Wall;
    using core::World;

    const Vector2 tileSize(0.2f, 0.2f);

    // --- Destroying a destructible wall with the Player's own bomb scores points. ---
    {
        auto score = std::make_shared<Score>();
        World world = makeEmptyWorldWithScore("world_score_tests_tmp1.txt", score);

        auto wall = std::make_unique<Wall>(Vector2(0.2f, 0.f), tileSize, true);
        auto bomb = std::make_unique<Bomb>(Vector2(0.f, 0.f), tileSize, CharacterColor::White, 1);
        const Bomb* bombPtr = bomb.get();

        world.addEntity(std::move(wall));
        world.addEntity(std::move(bomb));

        const EntityId bombId = bombPtr->getId();

        runner.check(score->getPoints() == 0, "No points scored before anything happens");
        world.detonateBomb(bombId, 1, CharacterColor::White);
        world.update(0.f);

        runner.check(score->getPoints() > 0, "Destroying a wall with the Player's own bomb scores points");
    }

    // --- Destroying a wall with a bot's bomb does not score the Player's Score. ---
    {
        auto score = std::make_shared<Score>();
        World world = makeEmptyWorldWithScore("world_score_tests_tmp2.txt", score);

        auto wall = std::make_unique<Wall>(Vector2(0.2f, 0.f), tileSize, true);
        auto bomb = std::make_unique<Bomb>(Vector2(0.f, 0.f), tileSize, CharacterColor::Blue, 1);
        const Bomb* bombPtr = bomb.get();

        world.addEntity(std::move(wall));
        world.addEntity(std::move(bomb));

        const EntityId bombId = bombPtr->getId();

        world.detonateBomb(bombId, 1, CharacterColor::Blue);
        world.update(0.f);

        runner.check(score->getPoints() == 0, "A bot destroying a wall does not score the Player's Score");
    }

    // --- Picking up a power-up scores points. ---
    {
        auto score = std::make_shared<Score>();
        World world = makeEmptyWorldWithScore("world_score_tests_tmp3.txt", score);

        auto character = std::make_unique<Character>(Vector2(0.f, 0.f), tileSize, CharacterColor::White);
        auto powerUp = std::make_unique<PowerUp>(Vector2(0.f, 0.f), tileSize, PowerUpType::Fire);
        const Character* characterPtr = character.get();

        world.addEntity(std::move(character));
        world.addEntity(std::move(powerUp));

        const EntityId characterId = characterPtr->getId();

        world.moveCharacter(characterId, Vector2(1.f, 0.f), 0.001f);
        world.update(0.f);

        runner.check(score->getPoints() > 0, "Picking up a power-up scores points");
    }

    // --- Killing an enemy with the Player's bomb scores points; killing yourself does not. ---
    {
        auto score = std::make_shared<Score>();
        World world = makeEmptyWorldWithScore("world_score_tests_tmp4.txt", score);

        auto enemy = std::make_unique<Character>(Vector2(0.2f, 0.f), tileSize, CharacterColor::Blue);
        auto bomb = std::make_unique<Bomb>(Vector2(0.f, 0.f), tileSize, CharacterColor::White, 1);
        const Bomb* bombPtr = bomb.get();

        world.addEntity(std::move(enemy));
        world.addEntity(std::move(bomb));

        const EntityId bombId = bombPtr->getId();

        world.detonateBomb(bombId, 1, CharacterColor::White);
        world.update(0.f);

        runner.check(score->getPoints() > 0, "Killing an enemy with the Player's own bomb scores points");
    }

    // --- The Player dying in their own blast does not score an "enemy killed" point. ---
    {
        auto score = std::make_shared<Score>();
        World world = makeEmptyWorldWithScore("world_score_tests_tmp5.txt", score);

        auto player = std::make_unique<Character>(Vector2(0.f, 0.f), tileSize, CharacterColor::White);
        auto bomb = std::make_unique<Bomb>(Vector2(0.f, 0.f), tileSize, CharacterColor::White, 1);
        const Bomb* bombPtr = bomb.get();

        world.addEntity(std::move(player));
        world.addEntity(std::move(bomb));

        const EntityId bombId = bombPtr->getId();

        world.detonateBomb(bombId, 1, CharacterColor::White);
        world.update(0.f);

        runner.check(score->getPoints() == 0, "The Player dying in their own blast is not scored as an enemy kill");
    }
}
