#include "CharacterColor.h"
#include "TestFactory.h"
#include "TestRunner.h"
#include "World.h"

#include <cstdio>
#include <fstream>
#include <memory>
#include <string>

namespace {

// Writes a tiny, deterministic (wall-only, no random cells) world file so the number of
// entities WorldLoader produces is predictable, then returns its path.
std::string writeDeterministicWorldFile() {
    const std::string path = "world_tests_tmp.txt";
    std::ofstream file(path);
    file << "WWW\n";
    file << "WWW\n";
    file << "WWW\n";
    return path;
}

} // namespace

void runWorldTests(tests::TestRunner& runner) {
    using core::Character;
    using core::CharacterColor;
    using core::EntityId;
    using core::Vector2;
    using core::World;

    const std::string path = writeDeterministicWorldFile();
    World world(std::make_shared<tests::TestFactory>(), path);
    std::remove(path.c_str());

    // The 3x3 all-'W' world produced exactly 9 Wall entities with ids 0..8; no player yet.
    runner.check(!world.getPlayerId().has_value(), "A world with no White character has no player id");

    world.addEntity(std::make_unique<Character>(Vector2(0.f, 0.f), Vector2(0.1f, 0.1f), CharacterColor::White));
    const std::optional<EntityId> playerId = world.getPlayerId();
    runner.check(playerId.has_value(), "Adding a White character sets the player id");
    runner.check(playerId.value_or(EntityId{999}) == 9, "The first entity added after 9 walls gets id 9");

    world.addEntity(std::make_unique<Character>(Vector2(0.f, 0.f), Vector2(0.1f, 0.1f), CharacterColor::Blue));
    runner.check(world.getPlayerId() == playerId,
                 "Adding a second (non-White) character does not change the player id");

    // Unknown ids must be handled gracefully (no crash, no effect) rather than assumed valid.
    world.moveCharacter(EntityId{999}, Vector2(1.f, 0.f), 0.016f);
    world.placeBomb(EntityId{999});
    runner.check(world.getPlayerId() == playerId, "Operating on an unknown entity id leaves world state unchanged");
}
