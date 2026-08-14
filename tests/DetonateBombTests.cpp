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

// Builds a World from a small deterministic (all-wall, no random cells) background, then removes
// every background entity, leaving a genuinely empty world with a known cellSize that a test can
// populate with its own precisely-positioned entities. nextEntityId keeps counting up past the
// removed background entities, so callers must not assume their own entities start at id 0 --
// capture each entity's raw pointer before moving it into the world and read its actual id back
// via getId() afterward (the pointee stays alive; only ownership moves).
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

void runDetonateBombTests(tests::TestRunner& runner) {
    using core::Bomb;
    using core::Character;
    using core::CharacterColor;
    using core::EntityId;
    using core::PowerUp;
    using core::Vector2;
    using core::Wall;
    using core::World;

    const Vector2 tileSize(0.2f, 0.2f);
    const Vector2 bombTile(0.f, 0.f);

    // --- An indestructible wall stops the blast; nothing beyond it is affected. ---
    {
        World world = makeEmptyWorld("detonate_tests_tmp1.txt");

        const Vector2 wallTile(0.2f, 0.f);   // immediately east of the bomb
        const Vector2 beyondTile(0.4f, 0.f); // further east, past the wall

        auto wall = std::make_unique<Wall>(wallTile, tileSize, false);
        auto beyondCharacter = std::make_unique<Character>(beyondTile, tileSize, CharacterColor::Blue);
        auto bomb = std::make_unique<Bomb>(bombTile, tileSize, CharacterColor::White, 3);

        const Wall* wallPtr = wall.get();
        const Character* beyondCharacterPtr = beyondCharacter.get();
        const Bomb* bombPtr = bomb.get();

        world.addEntity(std::move(wall));
        world.addEntity(std::move(beyondCharacter));
        world.addEntity(std::move(bomb));

        const EntityId wallId = wallPtr->getId();
        const EntityId beyondCharacterId = beyondCharacterPtr->getId();
        const EntityId bombId = bombPtr->getId();

        world.detonateBomb(bombId, 3, CharacterColor::White);
        world.update(0.f); // flush the deferred removals detonateBomb queued

        runner.check(world.hasEntity(wallId), "An indestructible wall survives an explosion");
        runner.check(world.hasEntity(beyondCharacterId), "Nothing beyond an indestructible wall is affected");
        runner.check(!world.hasEntity(bombId), "The detonated bomb itself is removed");
    }

    // --- A destructible wall stops the blast after being destroyed; only the nearest one. ---
    {
        World world = makeEmptyWorld("detonate_tests_tmp2.txt");

        const Vector2 nearWallTile(0.2f, 0.f);
        const Vector2 farWallTile(0.4f, 0.f);

        auto nearWall = std::make_unique<Wall>(nearWallTile, tileSize, true);
        auto farWall = std::make_unique<Wall>(farWallTile, tileSize, true);
        auto bomb = std::make_unique<Bomb>(bombTile, tileSize, CharacterColor::White, 3);

        const Wall* nearWallPtr = nearWall.get();
        const Wall* farWallPtr = farWall.get();
        const Bomb* bombPtr = bomb.get();

        world.addEntity(std::move(nearWall));
        world.addEntity(std::move(farWall));
        world.addEntity(std::move(bomb));

        const EntityId nearWallId = nearWallPtr->getId();
        const EntityId farWallId = farWallPtr->getId();
        const EntityId bombId = bombPtr->getId();

        world.detonateBomb(bombId, 3, CharacterColor::White);
        world.update(0.f);

        runner.check(!world.hasEntity(nearWallId), "The nearest destructible wall is destroyed");
        runner.check(world.hasEntity(farWallId),
                     "A second destructible wall further away survives -- the blast stops after one");
        runner.check(world.hasGrassAt(nearWallTile, tileSize),
                     "A destroyed destructible wall leaves grass behind on its tile");
    }

    // --- A character caught in the blast dies. ---
    {
        World world = makeEmptyWorld("detonate_tests_tmp3.txt");

        const Vector2 victimTile(0.f, 0.2f); // one tile south of the bomb

        auto victim = std::make_unique<Character>(victimTile, tileSize, CharacterColor::Blue);
        auto bomb = std::make_unique<Bomb>(bombTile, tileSize, CharacterColor::White, 2);

        const Character* victimPtr = victim.get();
        const Bomb* bombPtr = bomb.get();

        world.addEntity(std::move(victim));
        world.addEntity(std::move(bomb));

        const EntityId victimId = victimPtr->getId();
        const EntityId bombId = bombPtr->getId();

        world.detonateBomb(bombId, 2, CharacterColor::White);
        world.update(0.f);

        runner.check(!world.hasEntity(victimId), "A character caught in the blast dies");
    }

    // --- A power-up caught in the blast is destroyed. ---
    {
        World world = makeEmptyWorld("detonate_tests_tmp4.txt");

        const Vector2 powerUpTile(-0.2f, 0.f); // one tile west of the bomb

        auto powerUp = std::make_unique<PowerUp>(powerUpTile, tileSize, core::PowerUpType::Fire);
        auto bomb = std::make_unique<Bomb>(bombTile, tileSize, CharacterColor::White, 2);

        const PowerUp* powerUpPtr = powerUp.get();
        const Bomb* bombPtr = bomb.get();

        world.addEntity(std::move(powerUp));
        world.addEntity(std::move(bomb));

        const EntityId powerUpId = powerUpPtr->getId();
        const EntityId bombId = bombPtr->getId();

        world.detonateBomb(bombId, 2, CharacterColor::White);
        world.update(0.f);

        runner.check(!world.hasEntity(powerUpId), "A power-up caught in the blast is destroyed");
    }

    // --- A bomb caught in the blast chain-detonates, extending the cascade past the
    //     triggering bomb's own radius. ---
    {
        World world = makeEmptyWorld("detonate_tests_tmp5.txt");

        const Vector2 chainedBombTile(0.f, 0.2f); // one tile south of bomb A, within A's radius
        const Vector2 farWallTile(0.f, 0.6f);     // three tiles south of A -- only reachable via B's own blast

        auto chainedBomb = std::make_unique<Bomb>(chainedBombTile, tileSize, CharacterColor::Blue, 2); // radius 2
        auto farWall = std::make_unique<Wall>(farWallTile, tileSize, true);
        auto triggeringBomb = std::make_unique<Bomb>(bombTile, tileSize, CharacterColor::White, 1); // radius 1

        const Bomb* chainedBombPtr = chainedBomb.get();
        const Wall* farWallPtr = farWall.get();
        const Bomb* triggeringBombPtr = triggeringBomb.get();

        world.addEntity(std::move(chainedBomb));
        world.addEntity(std::move(farWall));
        world.addEntity(std::move(triggeringBomb));

        const EntityId chainedBombId = chainedBombPtr->getId();
        const EntityId farWallId = farWallPtr->getId();
        const EntityId triggeringBombId = triggeringBombPtr->getId();

        // Bomb A has radius 1, so its own blast alone cannot reach farWallTile (3 tiles away).
        // If it destroys that wall, it can only be because detonating A chain-triggered B (radius 2).
        world.detonateBomb(triggeringBombId, 1, CharacterColor::White);
        world.update(0.f);

        runner.check(!world.hasEntity(triggeringBombId), "The triggering bomb is removed");
        runner.check(!world.hasEntity(chainedBombId), "The bomb caught in the blast chain-detonates and is removed");
        runner.check(!world.hasEntity(farWallId),
                     "The chain-detonated bomb's own blast reaches beyond the triggering bomb's radius");
    }
}
