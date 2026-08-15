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
        runner.check(bomb.isBomb(), "A Bomb reports isBomb() true, for chain-reaction detection");

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

    // --- Capacity enforcement: World::placeBomb respects the character's own maxBombs. ---
    {
        const std::string path = writeDeterministicWorldFile();
        World world(std::make_shared<tests::TestFactory>(), path);
        std::remove(path.c_str());

        auto owner = std::make_unique<Character>(tilePosition, tileSize, CharacterColor::White);
        Character* ownerPtr = owner.get();
        const EntityId ownerId = 9; // first entity added after the 9 walls
        world.addEntity(std::move(owner));

        runner.check(ownerPtr->canPlaceBomb(), "A fresh character with maxBombs=1 can place a bomb");

        world.placeBomb(ownerId);
        const EntityId firstBombId = 10;
        runner.check(world.hasEntity(firstBombId), "placeBomb created a bomb entity for the character");
        runner.check(!ownerPtr->canPlaceBomb(), "After placing its one bomb, the character has no free slot left");

        world.placeBomb(ownerId); // at capacity: should be a no-op
        runner.check(!world.hasEntity(EntityId{11}),
                     "A second placeBomb call while at capacity does not create another bomb");

        // Step off the bomb's tile first: since detonateBomb now genuinely kills anything caught
        // in the blast, staying put would kill the owner along with despawning the bomb.
        ownerPtr->setPosition(Vector2(0.6f, 0.6f));

        world.update(3.f); // let the first bomb's fuse expire, freeing the slot
        runner.check(!world.hasEntity(firstBombId), "The first bomb has despawned after its fuse expired");
        runner.check(world.hasEntity(ownerId), "The owner survives its own bomb once it has stepped away");
        runner.check(ownerPtr->canPlaceBomb(), "The bomb slot is freed once the bomb it was holding is gone");

        world.placeBomb(ownerId);
        runner.check(world.hasEntity(EntityId{11}), "The character can place a new bomb once its slot is free again");
    }

    // --- A bystander already standing on a bomb's tile when it's placed -- e.g. an enemy that
    //     walked onto the player's tile and dropped one there, since characters don't block each
    //     other's movement -- gets the same grace period the owner does, rather than being
    //     trapped in place until the bomb explodes. ---
    {
        const std::string path = writeDeterministicWorldFile();
        World world(std::make_shared<tests::TestFactory>(), path);
        std::remove(path.c_str());

        auto bystander = std::make_unique<Character>(tilePosition, tileSize, CharacterColor::Blue);
        Character* bystanderPtr = bystander.get();
        world.addEntity(std::move(bystander));

        auto outsider = std::make_unique<Character>(Vector2(0.6f, 0.6f), tileSize, CharacterColor::Red);
        Character* outsiderPtr = outsider.get();
        world.addEntity(std::move(outsider));

        // The owner (White) is never actually placed in the world here -- only the bomb, dropped
        // by an enemy directly onto the bystander's tile.
        auto bomb = std::make_unique<Bomb>(tilePosition, tileSize, CharacterColor::White, 1);
        const Bomb* bombPtr = bomb.get();
        world.addEntity(std::move(bomb));

        world.update(0.016f); // lets the bomb's first onTick scan for bystanders on its tile

        runner.check(!bombPtr->blocksCharacterMovement(*bystanderPtr, tilePosition, tileSize),
                     "A bystander caught under a freshly placed bomb is not blocked by it");
        runner.check(bombPtr->blocksCharacterMovement(*outsiderPtr, tilePosition, tileSize),
                     "A character that was never on the bomb's tile is still blocked by it, as normal");

        // The bystander steps off, then back on: once gone, the exemption is permanent -- exactly
        // like the owner's -- so stepping back doesn't restore it.
        bystanderPtr->setPosition(Vector2(0.6f, 0.f));
        world.update(0.016f);
        bystanderPtr->setPosition(tilePosition);

        runner.check(bombPtr->blocksCharacterMovement(*bystanderPtr, tilePosition, tileSize),
                     "Once a bystander leaves the tile, the bomb blocks it again, even if it steps back");
    }
}
