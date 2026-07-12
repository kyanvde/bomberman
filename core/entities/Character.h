#ifndef BOMBERMAN_CORE_ENTITIES_CHARACTER_H
#define BOMBERMAN_CORE_ENTITIES_CHARACTER_H

#include "EntityModel.h"

namespace core {

class Character : public EntityModel {
public:
    Character(const Vector2& pos, const Vector2& size);

    [[nodiscard]] bool isPlayerControlled() const noexcept override { return true; }

    [[nodiscard]] bool isBlockedBy(const EntityModel& obstacle, const Vector2& position, const Vector2& size) const override;
    
};

}; // namespace core

#endif // BOMBERMAN_CORE_ENTITIES_CHARACTER_H