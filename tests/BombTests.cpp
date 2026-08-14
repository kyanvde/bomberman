#include "CharacterColor.h"
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

std::string writeDeterministicWorldFile() {
    const std::string path = "bomb_tests_tmp.txt";
    std::ofstream file(path);
    file << "WWW\n";
    file << "WWW\n";
    file << "WWW\n";
    return path;
}

} // namespace

void runBombTests(tests::TestRunner& runner) {
    using core::Bomb;
    using core::Character;
    using core::CharacterColor;
    using core::EntityId;
    using core::Vector2;
    using core::World;

    const Vector2 tilePosition(0.f, 0.f);
    const Vector2 tileSize(0.2f, 0.2f);

    // --- Owner-exception and arming behavior, driven through a real World so onTick fires. ---
    {
        const std::string path = writeDeterministicWorldFile();
        World world(std::make_shared<tests::TestFactory>(), path);
        std::remove(path.c_str());

        // 9 walls loaded first (ids 0..8); owner character gets id 9, a bystander gets id 10.
        auto owner = std::make_unique<Character>(tilePosition, tileSize, CharacterColor::White);
        Character* ownerPtr = owner.get();
        world.addEntity(std::move(owner));

        auto bystander = std::make_unique<Character>(Vector2(0.6f, 0.6f), tileSize, CharacterColor::Blue);
        world.addEntity(std::move(bystander));

        runner.check(world.isTileOccupiedByColor(tilePosition, tileSize, CharacterColor::White),
                     "isTileOccupiedByColor finds the owner standing on the bomb's tile");
        runner.check(!world.isTileOccupiedByColor(tilePosition, tileSize, CharacterColor::Red),
                     "isTileOccupiedByColor does not find a color that isn't present");

        Bomb bomb(tilePosition, tileSize, CharacterColor::White, 1);
        runner.check(!bomb.blocksCharacterMovement(*ownerPtr, tilePosition, tileSize),
                     "An unarmed bomb does not block its own owner");

        const Character otherColorCharacter(Vector2(0.f, 0.f), tileSize, CharacterColor::Blue);
        runner.check(bomb.blocksCharacterMovement(otherColorCharacter, tilePosition, tileSize),
                     "An unarmed bomb still blocks a character that isn't its owner");

        // Drive arming via a real Bomb owned by World, since `armed` is private and only World's
        // onTick dispatch can flip it. Owner is still standing on the tile at this point.
        ownerPtr->setPosition(tilePosition);
        auto worldBomb = std::make_unique<Bomb>(tilePosition, tileSize, CharacterColor::White, 1);
        const Bomb* worldBombPtr = worldBomb.get();
        world.addEntity(std::move(worldBomb));

        world.update(0.016f);
        runner.check(!worldBombPtr->blocksCharacterMovement(*ownerPtr, tilePosition, tileSize),
                     "While the owner is still standing on its tile, the bomb stays unarmed after a tick");

        // Owner leaves the tile; the next tick should observe this and arm the bomb.
        ownerPtr->setPosition(Vector2(0.6f, 0.6f));
        world.update(0.016f);
        runner.check(worldBombPtr->blocksCharacterMovement(*ownerPtr, tilePosition, tileSize),
                     "Once armed (owner has left its tile), the bomb blocks its own owner too");
    }

    // --- Fuse expiry: the bomb removes itself once its fuse runs out. ---
    {
        const std::string path = writeDeterministicWorldFile();
        World world(std::make_shared<tests::TestFactory>(), path);
        std::remove(path.c_str());

        auto bomb = std::make_unique<Bomb>(tilePosition, tileSize, CharacterColor::White, 1);
        const EntityId bombId = 9; // first entity added after the 9 walls
        world.addEntity(std::move(bomb));
        runner.check(world.hasEntity(bombId), "A freshly placed bomb is present in the world");

        world.update(0.5f);
        runner.check(world.hasEntity(bombId), "The bomb is still present before its fuse has fully counted down");

        world.update(2.f); // well past the fuse duration
        runner.check(!world.hasEntity(bombId), "The bomb removes itself once its fuse expires");
    }
}
