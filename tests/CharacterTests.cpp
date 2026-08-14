#include "CharacterColor.h"
#include "TestRunner.h"
#include "entities/Character.h"

void runCharacterTests(tests::TestRunner& runner) {
    using core::Character;
    using core::CharacterColor;
    using core::Vector2;

    const Character white(Vector2(-1.f, -1.f), Vector2(0.1f, 0.1f), CharacterColor::White);
    runner.check(white.isPlayerControlled(), "A White character is player-controlled");
    runner.check(white.getColor() == CharacterColor::White, "A White character reports its own color");
    runner.check(white.getCharacterColor() == CharacterColor::White, "getCharacterColor() matches getColor()");
    runner.check(white.isKilledByExplosion(), "A character dies when an explosion reaches it");
    runner.check(!white.blocksExplosion(), "A character does not block explosion propagation");
    runner.check(!white.isBomb(), "A character is not a bomb");

    const Character blue(Vector2(-1.f, -1.f), Vector2(0.1f, 0.1f), CharacterColor::Blue);
    runner.check(!blue.isPlayerControlled(), "A Blue character is not player-controlled");

    const Character red(Vector2(-1.f, -1.f), Vector2(0.1f, 0.1f), CharacterColor::Red);
    runner.check(!red.isPlayerControlled(), "A Red character is not player-controlled");

    const Character black(Vector2(-1.f, -1.f), Vector2(0.1f, 0.1f), CharacterColor::Black);
    runner.check(!black.isPlayerControlled(), "A Black character is not player-controlled");
}
