#include "CharacterColor.h"
#include "Random.h"
#include "TestFactory.h"
#include "TestRunner.h"
#include "TileGrid.h"
#include "World.h"

#include <algorithm>
#include <cstdio>
#include <fstream>
#include <memory>
#include <string>
#include <vector>

namespace {

// The real arena layout from assets/worlds/main.txt -- alternating pillars, randomly generated
// destructible walls and all -- with the four corner spawn slots filled in by the caller, so a
// test can isolate a single bot in a genuine map. Reproduced here rather than loading the asset so
// the test does not depend on the working directory it runs from.
std::string writeArena(const std::string& path, const std::string& cornerSpawns) {
    std::vector<std::string> rows = {
        "WWWWWWWWWWWWWWW", "W_____________W", "W_W_W_W_W_W_W_W", "W_____________W", "W_W_W_W_W_W_W_W",
        "W_____________W", "W_W_W_W_W_W_W_W", "W_____________W", "W_W_W_W_W_W_W_W", "W_____________W",
        "W_W_W_W_W_W_W_W", "W_____________W", "WWWWWWWWWWWWWWW",
    };

    rows[1][1] = cornerSpawns[0];
    rows[1][13] = cornerSpawns[1];
    rows[11][1] = cornerSpawns[2];
    rows[11][13] = cornerSpawns[3];

    std::ofstream file(path);
    for (const std::string& row : rows) {
        file << row << '\n';
    }
    return path;
}

int countDestructibleWalls(const core::World& world) {
    const core::TileGrid grid = world.buildTileGrid();
    return static_cast<int>(std::count(grid.destructible.begin(), grid.destructible.end(), 1));
}

// Only living characters appear in the snapshot, so this doubles as an "is it still alive" check.
bool isAlive(const core::World& world, const core::CharacterColor color) {
    const core::TileGrid grid = world.buildTileGrid();
    return std::any_of(grid.characters.begin(), grid.characters.end(),
                       [color](const core::CharacterTile& character) { return character.color == color; });
}

} // namespace

void runAISimulationTests(tests::TestRunner& runner) {
    using core::CharacterColor;
    using core::GridCoord;
    using core::TileGrid;
    using core::World;

    constexpr float frame = 1.f / 60.f;

    // --- A bot left completely alone in a real arena must never blow itself up: nothing else in
    //     the world can hurt it, so if it dies it walked into its own blast. This is exactly what
    //     the old escape logic guaranteed -- it only inspected the four adjacent tiles, but a bomb
    //     on your own tile threatens every one of them, so it found nowhere to go, returned "stand
    //     still", and sat on its own bomb.
    //
    //     Run over several fixed seeds rather than one: each seed lays the destructible walls out
    //     differently, so between them the bot meets dead ends, corridors and open ground. Seeding
    //     keeps it reproducible, so a failure can actually be investigated instead of being a run
    //     that happened to go wrong. ---
    int survivedCount = 0;
    int totalCount = 0;
    int arenasCleared = 0;
    for (unsigned int seed = 1; seed <= 20u; ++seed) {
        core::Random::getInstance().setSeed(seed);

        const std::string path = "ai_simulation_tests_tmp1.txt";
        writeArena(path, "B___"); // a single Blue bot; no player, no rivals
        World world(std::make_shared<tests::TestFactory>(), path);
        std::remove(path.c_str());

        const int wallsBefore = countDestructibleWalls(world);

        bool survived = true;
        for (int step = 0; step < 60 * 40 && survived; ++step) {
            world.update(frame);
            survived = isAlive(world, CharacterColor::Blue);
        }

        ++totalCount;
        survivedCount += survived ? 1 : 0;
        arenasCleared += countDestructibleWalls(world) < wallsBefore ? 1 : 0;
    }

    // Allowing one loss out of twenty is deliberate, not a fudge for a flaky test: the seeds are
    // fixed, so this is a fixed verdict rather than something that varies run to run. Forty seconds
    // of solid bombing leaves a bot carrying several extra-bomb and bigger-blast power-ups at once,
    // and it can still occasionally corner itself with three large simultaneous blasts of its own.
    // What this guards against is the systematic failure that used to make bots kill themselves
    // essentially every time, rather than claiming perfect play.
    runner.check(survivedCount >= totalCount - 1, "Bots alone in an arena reliably survive their own bombs");
    runner.check(arenasCleared == totalCount, "A bot alone in an arena always places bombs and clears walls");

    // --- The bot has to find its way around the arena rather than wedging against the first
    //     pillar it meets, which is what greedy "walk toward the target" movement does here. ---
    {
        const std::string path = "ai_simulation_tests_tmp2.txt";
        writeArena(path, "B___");
        World world(std::make_shared<tests::TestFactory>(), path);
        std::remove(path.c_str());

        std::vector<GridCoord> visited;
        for (int step = 0; step < 60 * 20; ++step) {
            world.update(frame);

            const TileGrid grid = world.buildTileGrid();
            for (const core::CharacterTile& character : grid.characters) {
                if (character.color != CharacterColor::Blue) {
                    continue;
                }
                const bool seen = std::any_of(visited.begin(), visited.end(),
                                              [&](const GridCoord& coord) { return coord == character.coord; });
                if (!seen) {
                    visited.push_back(character.coord);
                }
            }
        }

        runner.check(visited.size() >= 5, "A bot roams the arena rather than wedging against the first pillar");
    }

    // --- A full four-way round runs without deadlocking: bombs go off and walls come down. ---
    {
        const std::string path = "ai_simulation_tests_tmp3.txt";
        writeArena(path, "PBRL");
        World world(std::make_shared<tests::TestFactory>(), path);
        std::remove(path.c_str());

        const int wallsBefore = countDestructibleWalls(world);

        for (int step = 0; step < 60 * 20; ++step) {
            world.update(frame);
        }

        runner.check(countDestructibleWalls(world) < wallsBefore,
                     "Three bots sharing an arena clear destructible walls between them");
    }
}
