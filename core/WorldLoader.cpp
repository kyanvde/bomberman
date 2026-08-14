#include "WorldLoader.h"

#include <algorithm>
#include <cstddef>
#include <fstream>
#include <stdexcept>
#include <vector>

#include "CharacterColor.h"
#include "Random.h"

namespace {

std::vector<std::string> getLinesFromFile(const std::string& filename) {
    std::vector<std::string> lines;
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + filename);
    }
    std::string line;
    while (std::getline(file, line)) {
        lines.push_back(line);
    }
    file.close();
    return lines;
}

bool verifyFileFormat(const std::vector<std::string>& lines) {
    if (lines.empty()) {
        return false;
    }

    const std::size_t lineLength = lines.front().length();
    return std::all_of(lines.begin(), lines.end(),
                       [lineLength](const std::string& line) { return line.length() == lineLength; });
}

bool isSpawnCell(const char cell) { return cell == 'P' || cell == 'B' || cell == 'R' || cell == 'L'; }

struct GridPosition {
    std::size_t x;
    std::size_t y;
};

// A destructible-wall roll this close (in the same row or column) to a spawn point is skipped in
// favor of always placing grass, so every character has a guaranteed two-tile clear path in at
// least one direction -- enough to escape their own radius-1 starting bomb -- instead of
// potentially spawning boxed in on every side by chance.
constexpr std::size_t spawnClearance = 2;

bool isNearSpawn(const std::vector<GridPosition>& spawns, const std::size_t x, const std::size_t y) {
    for (const GridPosition& spawn : spawns) {
        const std::size_t dx = spawn.x > x ? spawn.x - x : x - spawn.x;
        const std::size_t dy = spawn.y > y ? spawn.y - y : y - spawn.y;

        if ((spawn.x == x && dy <= spawnClearance) || (spawn.y == y && dx <= spawnClearance)) {
            return true;
        }
    }

    return false;
}

} // namespace

namespace core {
namespace {
// Most '_' cells become destructible walls; only a low chance spawns air (open grass) instead,
// matching the classic alternating-block Bomberman layout.
constexpr double destructibleWallChance = 0.85;
} // namespace

void WorldLoader::loadFromFile(const std::string& filename, World& world,
                               const std::shared_ptr<AbstractFactory>& factory) {
    if (!factory) {
        throw std::runtime_error("World factory is null while loading: " + filename);
    }

    const std::vector<std::string> lines = getLinesFromFile(filename);

    if (!verifyFileFormat(lines)) {
        throw std::runtime_error("Invalid world format: " + filename);
    }

    const Vector2 worldSize(static_cast<float>(lines[0].length()), static_cast<float>(lines.size()));

    const Vector2 cellSize(2.f / worldSize.x, 2.f / worldSize.y);
    world.setCellSize(cellSize);

    std::vector<GridPosition> spawns;
    for (std::size_t y = 0; y < lines.size(); ++y) {
        for (std::size_t x = 0; x < lines[y].length(); ++x) {
            if (isSpawnCell(lines[y][x])) {
                spawns.push_back({x, y});
            }
        }
    }

    for (std::size_t y = 0; y < lines.size(); ++y) {
        const std::string& line = lines[y];

        for (std::size_t x = 0; x < line.length(); ++x) {
            const char cell = line[x];
            const Vector2 position(-1.f + static_cast<float>(x) * cellSize.x,
                                   -1.f + static_cast<float>(y) * cellSize.y);
            const Vector2 size(cellSize.x, cellSize.y);
            // Entities in the row above are already loaded by the time this row is processed, so
            // the shading a grass tile needs (whether the tile directly above it is a wall) can be
            // read back from the world itself rather than tracked separately during loading.
            const Vector2 abovePosition(position.x, position.y - cellSize.y);
            const bool shaded = world.isWallAt(abovePosition, size);

            switch (cell) {
            case 'P':
                world.addEntity(factory->createGrass(position, size, shaded));
                world.addEntity(factory->createCharacter(position, size, CharacterColor::White));
                break;
            case 'B':
                world.addEntity(factory->createGrass(position, size, shaded));
                world.addEntity(factory->createCharacter(position, size, CharacterColor::Blue));
                break;
            case 'R':
                world.addEntity(factory->createGrass(position, size, shaded));
                world.addEntity(factory->createCharacter(position, size, CharacterColor::Red));
                break;
            case 'L':
                world.addEntity(factory->createGrass(position, size, shaded));
                world.addEntity(factory->createCharacter(position, size, CharacterColor::Black));
                break;
            case 'W':
                world.addEntity(factory->createWall(position, size, false));
                break;
            case '_':
                if (!isNearSpawn(spawns, x, y) && Random::getInstance().chance(destructibleWallChance)) {
                    world.addEntity(factory->createWall(position, size, true));
                } else {
                    world.addEntity(factory->createGrass(position, size, shaded));
                }
                break;
            default:
                throw std::runtime_error("Unknown cell type: " + std::string(1, cell));
            }
        }
    }
}

} // namespace core
