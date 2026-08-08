#ifndef BOMBERMAN_CORE_ENTITIES_GRASS_H
#define BOMBERMAN_CORE_ENTITIES_GRASS_H

#include "EntityModel.h"

namespace core {

/**
 * @brief The Grass class represents a grass entity in the game world.
 * It inherits from EntityModel and provides specific behavior for grass entities.
 */
class Grass final : public EntityModel {
public:
    /**
     * @brief Constructs a new Grass object with the specified position and size.
     * @param pos The position of the grass in world coordinates.
     * @param size The size of the grass in world coordinates.
     */
    Grass(const Vector2& pos, const Vector2& size);
};

}; // namespace core

#endif // BOMBERMAN_CORE_ENTITIES_GRASS_H