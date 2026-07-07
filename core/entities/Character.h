#ifndef BOMBERMAN_CORE_ENTITIES_CHARACTER_H
#define BOMBERMAN_CORE_ENTITIES_CHARACTER_H

#include "EntityModel.h"

namespace core {

class Character : public EntityModel {
public:
    Character(const Vector2& pos, const Vector2& size);
    
};

}; // namespace core

#endif // BOMBERMAN_CORE_ENTITIES_CHARACTER_H