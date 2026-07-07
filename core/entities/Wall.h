#ifndef BOMBERMAN_CORE_ENTITIES_WALL_H
#define BOMBERMAN_CORE_ENTITIES_WALL_H

#include "EntityModel.h"

namespace core {

class Wall : public EntityModel {
public:
    Wall(const Vector2& pos, const Vector2& size);
    
};

}; // namespace core

#endif // BOMBERMAN_CORE_ENTITIES_WALL_H