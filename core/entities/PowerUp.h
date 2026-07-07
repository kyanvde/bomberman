#ifndef BOMBERMAN_CORE_ENTITIES_POWERUP_H
#define BOMBERMAN_CORE_ENTITIES_POWERUP_H

#include "EntityModel.h"

namespace core {

class PowerUp : public EntityModel {
public:
    PowerUp(const Vector2& pos, const Vector2& size);
    
};

}; // namespace core

#endif // BOMBERMAN_CORE_ENTITIES_POWERUP_H