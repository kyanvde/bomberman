#include "WorldLoader.h"

#include <algorithm>
#include <fstream>
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

        unsigned int lineLength = lines[0].length();
        std::all_of(lines.begin(), lines.end(), [lineLength](const std::string& line) {
            return line.length() == lineLength;
        });
        return true;
    }

} // namespace

namespace core {

void WorldLoader::loadFromFile(const std::string& filename, World& world, std::shared_ptr<AbstractFactory>& factory) {
    const std::vector<std::string> lines = getLinesFromFile(filename);

    if (!verifyFileFormat(lines)) {
        throw std::runtime_error("Invalid world format: " + filename);
    }

    const Vector2 worldSize(static_cast<float>(lines[0].length()), static_cast<float>(lines.size()));

    const Vector2 cellSize(2.f / worldSize.x, 2.f / worldSize.y);

    for (unsigned int y = 0; y < lines.size(); ++y) {
        const std::string& line = lines[y];
        for (unsigned int x = 0; x < line.length(); ++x) {
            char cell = line[x];
            Vector2 position(-1.f + x * cellSize.x, -1.f + y * cellSize.y);
            Vector2 size(cellSize.x, cellSize.y);
            double result = 0;

            switch (cell) {
                case 'C':
                    world.addEntity(factory->createCharacter(position, size));
                    break;
                case 'W':
                    world.addEntity(factory->createWall(position, size));
                    break;
                case '_':
                    result = Random::getInstance().getRandomNumber(0, 1);
                    if (result < 0.75) {
                        world.addEntity(factory->createWall(position, size, true));
                    } else {
                        world.addEntity(factory->createGrass(position, size));
                    }
                    break;
                default:
                    throw std::runtime_error("Unknown cell type: " + std::string(1, cell));
            }
        }
    }
}

} // namespace core