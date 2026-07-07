#ifndef BOMBERMAN_CORE_WORLD_H
#define BOMBERMAN_CORE_WORLD_H

#include <memory>
#include "AbstractFactory.h"

namespace core {

class World {
private:
    std::shared_ptr<AbstractFactory> factory;    
};

} // namespace core

#endif // BOMBERMAN_CORE_WORLD_H