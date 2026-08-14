#ifndef BOMBERMAN_CORE_WORLDLOADER_H
#define BOMBERMAN_CORE_WORLDLOADER_H

#include "World.h"
#include <memory>
#include <string>

namespace core {

/**
 * @brief The WorldLoader class is responsible for loading a game world from a file.
 * It provides a static method to load the world data and populate the World object using the specified factory.
 */
class WorldLoader {
public:
    /**
     * @brief Loads a game world from a file and populates the provided World object using the specified factory.
     * @param filename The path to the file containing the world data.
     * @param world The World object to be populated with the loaded data.
     * @param factory A shared pointer to an AbstractFactory used for creating game entities.
     */
    static void loadFromFile(const std::string& filename, World& world,
                             const std::shared_ptr<AbstractFactory>& factory);
};

} // namespace core

#endif // BOMBERMAN_CORE_WORLDLOADER_H