#ifndef BOMBERMAN_CORE_ENTITIES_GRASS_H
#define BOMBERMAN_CORE_ENTITIES_GRASS_H

#include "EntityModel.h"

namespace core {

class Grass : public EntityModel {
public:
    Grass(const Vector2& pos, const Vector2& size);
};

}; // namespace core

#endif // BOMBERMAN_CORE_ENTITIES_GRASS_H