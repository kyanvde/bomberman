#ifndef BOMBERMAN_CORE_ENTITIES_WALL_H
#define BOMBERMAN_CORE_ENTITIES_WALL_H

#include "EntityModel.h"

namespace core {

class Wall : public EntityModel {
public:
    Wall(const Vector2& pos, const Vector2& size, bool destructible = false);

    [[nodiscard]] bool blocksCharacterMovement(const Character& character, const Vector2& position, const Vector2& size) const override;
    
};

}; // namespace core

#endif // BOMBERMAN_CORE_ENTITIES_WALL_H