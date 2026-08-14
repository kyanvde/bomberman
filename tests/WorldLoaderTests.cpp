#include "CharacterColor.h"
#include "TestFactory.h"
#include "TestRunner.h"
#include "World.h"

#include <cstdio>
#include <fstream>
#include <memory>
#include <string>
#include <vector>

namespace {

std::string writeWorldFile(const std::string& path, const std::vector<std::string>& rows) {
    std::ofstream file(path);
    for (const std::string& row : rows) {
        file << row << '\n';
    }
    return path;
}

} // namespace

void runWorldLoaderTests(tests::TestRunner& runner) {
    using core::Vector2;
    using core::World;

    // --- Every cell within 2 tiles of a spawn point, along its row or column, is guaranteed
    //     clear (never a destructible wall) -- enough for a character to escape a radius-1 bomb
    //     placed on their own spawn tile in at least one direction, regardless of the random roll. ---
    {
        const std::vector<std::string> rows = {
            "WWWWWWWWW", "W_______W", "W_______W", "W__P____W", "W_______W", "W_______W", "WWWWWWWWW",
        };
        const std::string path = "world_loader_tests_tmp1.txt";
        writeWorldFile(path, rows);

        World world(std::make_shared<tests::TestFactory>(), path);
        std::remove(path.c_str());

        const Vector2 cellSize(2.f / static_cast<float>(rows[0].length()), 2.f / static_cast<float>(rows.size()));
        const auto tileTopLeft = [&](const int col, const int row) {
            return Vector2(-1.f + static_cast<float>(col) * cellSize.x, -1.f + static_cast<float>(row) * cellSize.y);
        };

        constexpr int spawnCol = 3;
        constexpr int spawnRow = 3;
        constexpr int offsets[] = {-2, -1, 1, 2};

        for (const int offset : offsets) {
            runner.check(!world.isWallAt(tileTopLeft(spawnCol + offset, spawnRow), cellSize),
                         "A cell within 2 tiles of a spawn, same row, is never a wall");
            runner.check(!world.isWallAt(tileTopLeft(spawnCol, spawnRow + offset), cellSize),
                         "A cell within 2 tiles of a spawn, same column, is never a wall");
        }
    }

    // --- The clearance fix doesn't disable wall generation entirely -- destructible walls can
    //     still appear well away from any spawn's clearance zone. ---
    {
        std::vector<std::string> rows;
        rows.emplace_back(24, 'W');
        for (int i = 0; i < 8; ++i) {
            std::string row(24, '_');
            row.front() = 'W';
            row.back() = 'W';
            rows.push_back(row);
        }
        rows.back() = std::string(24, 'W');
        rows[1][1] = 'P'; // spawn near the top-left corner; far side of the map is untouched by it

        const std::string path = "world_loader_tests_tmp2.txt";
        writeWorldFile(path, rows);

        World world(std::make_shared<tests::TestFactory>(), path);
        std::remove(path.c_str());

        const Vector2 cellSize(2.f / static_cast<float>(rows[0].length()), 2.f / static_cast<float>(rows.size()));
        const auto tileTopLeft = [&](const int col, const int row) {
            return Vector2(-1.f + static_cast<float>(col) * cellSize.x, -1.f + static_cast<float>(row) * cellSize.y);
        };

        bool foundWallFarFromSpawn = false;
        for (int col = 15; col < 23 && !foundWallFarFromSpawn; ++col) {
            for (int row = 1; row < 8 && !foundWallFarFromSpawn; ++row) {
                if (world.isWallAt(tileTopLeft(col, row), cellSize)) {
                    foundWallFarFromSpawn = true;
                }
            }
        }

        runner.check(foundWallFarFromSpawn,
                     "Destructible walls still generate away from spawn clearance zones (fix isn't overbroad)");
    }
}
