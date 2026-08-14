#include "CharacterColor.h"
#include "GameEvent.h"
#include "Score.h"
#include "TestRunner.h"

void runScoreTests(tests::TestRunner& runner) {
    using core::CharacterColor;
    using core::GameEvent;
    using core::GameEventType;
    using core::Score;

    runner.check(Score().getPoints() == 0, "A fresh Score starts at 0 points");

    // --- Events attributed to the Player (White) are counted. ---
    {
        Score score;
        score.update(GameEvent{GameEventType::BlockDestroyed, CharacterColor::White});
        const int afterBlock = score.getPoints();
        runner.check(afterBlock > 0, "Destroying a block attributed to the Player earns points");

        score.update(GameEvent{GameEventType::PowerUpCollected, CharacterColor::White});
        runner.check(score.getPoints() > afterBlock, "Collecting a power-up earns additional points");

        const int beforeKill = score.getPoints();
        score.update(GameEvent{GameEventType::EntityKilled, CharacterColor::White});
        runner.check(score.getPoints() > beforeKill, "Killing an enemy earns additional points");
    }

    // --- Events attributed to a bot are ignored -- only the Player's own actions score. ---
    {
        Score score;
        score.update(GameEvent{GameEventType::BlockDestroyed, CharacterColor::Blue});
        score.update(GameEvent{GameEventType::EntityKilled, CharacterColor::Red});
        runner.check(score.getPoints() == 0, "Events attributed to a bot don't affect the Player's score");
    }

    // --- Winning grants a bonus; losing applies a penalty (never dropping below 0). ---
    {
        Score wonScore;
        wonScore.update(GameEvent{GameEventType::GameWon, CharacterColor::White});
        runner.check(wonScore.getPoints() > 0, "Winning grants a positive bonus");

        Score lostScore;
        lostScore.update(GameEvent{GameEventType::GameLost, CharacterColor::White});
        runner.check(lostScore.getPoints() == 0, "Losing from a score of 0 never drops below 0");
    }

    // --- Time alive is accumulated via tick(), independently of the Observer channel. ---
    {
        Score score;
        runner.check(score.getPoints() == 0, "No time has passed yet");
        score.tick(5.f);
        runner.check(score.getPoints() > 0, "Points accumulate the longer the round has been alive");
    }

    // --- A routine Tick GameEvent (the default every entity sends on every position/animation
    //     change) does not itself score anything -- only tick() does. ---
    {
        Score score;
        score.update(GameEvent{GameEventType::Tick, CharacterColor::White});
        runner.check(score.getPoints() == 0, "A routine Tick notification does not score any points on its own");
    }
}
