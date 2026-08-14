#include "HighScores.h"
#include "TestRunner.h"

#include <cstdio>
#include <fstream>
#include <string>

void runHighScoresTests(tests::TestRunner& runner) {
    using core::HighScores;

    // --- A missing file is treated as an empty list, not an error. ---
    {
        const std::string path = "highscores_tests_missing.txt";
        std::remove(path.c_str());

        const HighScores highScores(path);
        runner.check(highScores.getScores().empty(), "A missing high score file starts as an empty list");
    }

    // --- A corrupt file (non-numeric lines) is tolerated: bad lines are skipped rather than
    //     treated as fatal. ---
    {
        const std::string path = "highscores_tests_corrupt.txt";
        {
            std::ofstream file(path);
            file << "not a number\n";
            file << "150\n";
            file << "\n";
            file << "also not a number\n";
        }

        const HighScores highScores(path);
        std::remove(path.c_str());

        runner.check(highScores.getScores().size() == 1, "Only the valid numeric line survives a corrupt file");
        runner.check(highScores.getScores().front() == 150, "The surviving score is parsed correctly");
    }

    // --- Recording scores keeps the list sorted descending and capped at the top 5. ---
    {
        const std::string path = "highscores_tests_record.txt";
        std::remove(path.c_str());

        HighScores highScores(path);
        highScores.record(10);
        highScores.record(50);
        highScores.record(30);
        highScores.record(20);
        highScores.record(40);
        highScores.record(5); // sixth entry -- should be dropped, it's the lowest

        const std::vector<int>& scores = highScores.getScores();
        runner.check(scores.size() == 5, "The high score list is capped at 5 entries");
        runner.check(scores.front() == 50, "The highest score is first");
        runner.check(scores.back() == 10, "The lowest surviving score is last, and 5 was dropped");

        bool sortedDescending = true;
        for (std::size_t i = 1; i < scores.size(); ++i) {
            if (scores[i] > scores[i - 1]) {
                sortedDescending = false;
            }
        }
        runner.check(sortedDescending, "The high score list is sorted descending");

        // --- Recorded scores persist to disk: a fresh HighScores reloads the same list. ---
        const HighScores reloaded(path);
        std::remove(path.c_str());
        runner.check(reloaded.getScores() == scores, "A freshly-loaded HighScores sees the previously recorded list");
    }
}
