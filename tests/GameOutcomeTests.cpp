#include "CharacterColor.h"
#include "GameOutcome.h"
#include "TestFactory.h"
#include "TestRunner.h"
#include "World.h"
#include "entities/Bomb.h"
#include "entities/Character.h"

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

void runGameOutcomeTests(tests::TestRunner& runner) {
    using core::Bomb;
    using core::Character;
    using core::CharacterColor;
    using core::EntityId;
    using core::GameOutcome;
    using core::Vector2;
    using core::World;

    const Vector2 tileSize(0.2f, 0.2f);

    // --- No characters at all yet reported dead: in progress. ---
    {
        World world = makeEmptyWorld("outcome_tests_tmp1.txt");

        auto player = std::make_unique<Character>(Vector2(0.f, 0.f), tileSize, CharacterColor::White);
        auto bot = std::make_unique<Character>(Vector2(0.4f, 0.f), tileSize, CharacterColor::Blue);
        world.addEntity(std::move(player));
        world.addEntity(std::move(bot));

        runner.check(world.getOutcome() == GameOutcome::InProgress,
                     "The round is in progress while both the Player and at least one bot are alive");
    }

    // --- Every bot dead, Player alive: the Player has won. ---
    {
        World world = makeEmptyWorld("outcome_tests_tmp2.txt");

        auto player = std::make_unique<Character>(Vector2(0.f, 0.f), tileSize, CharacterColor::White);
        auto bot = std::make_unique<Character>(Vector2(0.4f, 0.f), tileSize, CharacterColor::Blue);
        Character* botPtr = bot.get();

        world.addEntity(std::move(player));
        world.addEntity(std::move(bot));

        botPtr->onExplosionKill();

        runner.check(world.getOutcome() == GameOutcome::PlayerWon, "The Player wins once every bot is dead");
    }

    // --- Player dead, a bot still alive: the Player has lost. ---
    {
        World world = makeEmptyWorld("outcome_tests_tmp3.txt");

        auto player = std::make_unique<Character>(Vector2(0.f, 0.f), tileSize, CharacterColor::White);
        auto bot = std::make_unique<Character>(Vector2(0.4f, 0.f), tileSize, CharacterColor::Blue);
        Character* playerPtr = player.get();

        world.addEntity(std::move(player));
        world.addEntity(std::move(bot));

        playerPtr->onExplosionKill();

        runner.check(world.getOutcome() == GameOutcome::PlayerLost,
                     "The Player loses if dead, even while a bot remains alive");
    }

    // --- Both the Player and the last bot die together: resolves as a loss, not a win. ---
    {
        World world = makeEmptyWorld("outcome_tests_tmp4.txt");

        auto player = std::make_unique<Character>(Vector2(0.f, 0.f), tileSize, CharacterColor::White);
        auto bot = std::make_unique<Character>(Vector2(0.4f, 0.f), tileSize, CharacterColor::Blue);
        Character* playerPtr = player.get();
        Character* botPtr = bot.get();

        world.addEntity(std::move(player));
        world.addEntity(std::move(bot));

        playerPtr->onExplosionKill();
        botPtr->onExplosionKill();

        runner.check(world.getOutcome() == GameOutcome::PlayerLost,
                     "A simultaneous Player/last-bot death resolves as a Player loss");
    }

    // --- A dead character stops responding to movement input and can no longer place bombs. ---
    {
        World world = makeEmptyWorld("outcome_tests_tmp5.txt");

        auto character = std::make_unique<Character>(Vector2(0.f, 0.f), tileSize, CharacterColor::White);
        Character* characterPtr = character.get();
        world.addEntity(std::move(character));

        const EntityId characterId = characterPtr->getId();
        characterPtr->onExplosionKill();

        const Vector2 positionBefore = characterPtr->getPosition();
        world.moveCharacter(characterId, Vector2(1.f, 0.f), 0.5f);
        const Vector2 positionAfter = characterPtr->getPosition();

        runner.check(positionBefore.x == positionAfter.x && positionBefore.y == positionAfter.y,
                     "A dead character does not move in response to movement input");
        runner.check(!characterPtr->canPlaceBomb(), "A dead character can no longer place a bomb");
    }

    // --- A bomb caught in a blast still kills the character caught with it, via the same path. ---
    {
        World world = makeEmptyWorld("outcome_tests_tmp6.txt");

        const Vector2 bombTile(0.f, 0.f);
        const Vector2 victimTile(0.2f, 0.f);

        auto victim = std::make_unique<Character>(victimTile, tileSize, CharacterColor::Blue);
        auto bomb = std::make_unique<Bomb>(bombTile, tileSize, CharacterColor::White, 1);

        Character* victimPtr = victim.get();
        const Bomb* bombPtr = bomb.get();

        world.addEntity(std::move(victim));
        world.addEntity(std::move(bomb));

        const EntityId bombId = bombPtr->getId();

        world.detonateBomb(bombId, 1, CharacterColor::White);
        world.update(0.f);

        runner.check(!victimPtr->isAlive(), "A character caught in a real blast is killed via onExplosionKill");
    }
}
