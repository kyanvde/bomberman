#ifndef BOMBERMAN_CORE_ENTITIES_EXPLOSION_H
#define BOMBERMAN_CORE_ENTITIES_EXPLOSION_H

#include "EntityModel.h"

namespace core {

/**
 * @brief The Explosion class represents a single affected tile of a bomb blast in the game
 * world. It inherits from EntityModel and exists only briefly, self-removing once its lifetime
 * has elapsed. It does not block movement or explosions, is not itself destructible, and takes
 * no part in propagation -- it is a purely visual/timing marker for one blast tile, created by
 * World::detonateBomb.
 */
class Explosion final : public EntityModel {
    /**
     * @brief The time remaining, in seconds, before this explosion removes itself.
     */
    float remainingLifetime;

public:
    /**
     * @brief Constructs a new Explosion object with the specified position and size.
     * @param pos The position of the explosion in world coordinates.
     * @param size The size of the explosion in world coordinates.
     */
    Explosion(const Vector2& pos, const Vector2& size);

    /**
     * @brief Explosions render above characters, so they remain visible over anything caught in
     * the blast.
     */
    [[nodiscard]] int renderLayer() const noexcept override;

    /**
     * @brief Counts down this explosion's remaining lifetime, removing itself once it expires.
     * @param world The world this explosion belongs to.
     * @param selfId This explosion's own identifier.
     * @param deltaTime The time elapsed since the previous tick, in seconds.
     */
    void onTick(World& world, EntityId selfId, float deltaTime) override;
};

}; // namespace core

#endif // BOMBERMAN_CORE_ENTITIES_EXPLOSION_H
