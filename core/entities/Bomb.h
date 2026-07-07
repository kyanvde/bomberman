#ifndef BOMBERMAN_CORE_ENTITIES_BOMB_H
#define BOMBERMAN_CORE_ENTITIES_BOMB_H

#include "EntityModel.h"

namespace core {

class Bomb : public EntityModel {
public:
    Bomb(const Vector2& pos, const Vector2& size);
    
};

}; // namespace core

#endif // BOMBERMAN_CORE_ENTITIES_BOMB_H