#include "CharacterColor.h"
#include "PowerUpType.h"
#include "TestFactory.h"
#include "TestRunner.h"
#include "World.h"
#include "entities/Character.h"
#include "entities/PowerUp.h"

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

void runPowerUpPickupTests(tests::TestRunner& runner) {
    using core::Character;
    using core::CharacterColor;
    using core::EntityId;
    using core::PowerUp;
    using core::PowerUpType;
    using core::Vector2;
    using core::World;

    const Vector2 tileSize(0.2f, 0.2f);

    // --- Walking onto a power-up's tile picks it up: the stat boost is applied and the
    //     power-up entity is removed. ---
    {
        World world = makeEmptyWorld("powerup_pickup_tests_tmp1.txt");

        auto character = std::make_unique<Character>(Vector2(0.f, 0.f), tileSize, CharacterColor::White);
        auto powerUp = std::make_unique<PowerUp>(Vector2(0.f, 0.f), tileSize, PowerUpType::Fire);

        const Character* characterPtr = character.get();
        const PowerUp* powerUpPtr = powerUp.get();

        world.addEntity(std::move(character));
        world.addEntity(std::move(powerUp));

        const EntityId characterId = characterPtr->getId();
        const EntityId powerUpId = powerUpPtr->getId();

        runner.check(characterPtr->getBombRadius() == 1, "Before pickup, the character has the default bomb radius");

        // A tiny movement is enough to re-run the overlap check while staying on the same tile as
        // the power-up (which starts already fully overlapping the character).
        world.moveCharacter(characterId, Vector2(1.f, 0.f), 0.001f);
        world.update(0.f); // flush the deferred removal moveCharacter queued

        runner.check(characterPtr->getBombRadius() == 2, "Walking onto a Fire power-up increases bomb radius");
        runner.check(!world.hasEntity(powerUpId), "A picked-up power-up is removed from the world");
    }

    // --- A power-up the character never overlaps is left alone. ---
    {
        World world = makeEmptyWorld("powerup_pickup_tests_tmp2.txt");

        auto character = std::make_unique<Character>(Vector2(0.f, 0.f), tileSize, CharacterColor::White);
        auto powerUp = std::make_unique<PowerUp>(Vector2(0.6f, 0.6f), tileSize, PowerUpType::ExtraBomb);

        const Character* characterPtr = character.get();
        const PowerUp* powerUpPtr = powerUp.get();

        world.addEntity(std::move(character));
        world.addEntity(std::move(powerUp));

        const EntityId characterId = characterPtr->getId();
        const EntityId powerUpId = powerUpPtr->getId();

        world.moveCharacter(characterId, Vector2(1.f, 0.f), 0.001f);
        world.update(0.f);

        runner.check(world.hasEntity(powerUpId), "A power-up the character never touches stays in the world");
        runner.check(characterPtr->canPlaceBomb(), "The character's stats are unaffected by a distant power-up");
    }
}
