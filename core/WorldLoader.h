#ifndef BOMBERMAN_CORE_WORLDLOADER_H
#define BOMBERMAN_CORE_WORLDLOADER_H

#include <string>
#include "World.h"
#include <memory>

namespace core {

class WorldLoader {
public:
    static void loadFromFile(const std::string& filename, World& world, std::shared_ptr<AbstractFactory>& factory);
};

} // namespace core

#endif // BOMBERMAN_CORE_WORLDLOADER_H