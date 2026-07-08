#ifndef BOMBERMAN_CORE_ABSTRACTFACTORY_H
#define BOMBERMAN_CORE_ABSTRACTFACTORY_H

#include <memory>
#include "EntityModel.h"
#include "Vector2.h"

namespace core {

class AbstractFactory {
public:
    virtual std::unique_ptr<EntityModel> createCharacter(const Vector2& pos, const Vector2& size) = 0;

    virtual std::unique_ptr<EntityModel> createWall(const Vector2& pos, const Vector2& size, bool destructible = false) = 0;
    
    virtual std::unique_ptr<EntityModel> createPowerUp(const Vector2& pos, const Vector2& size) = 0;
    
    virtual std::unique_ptr<EntityModel> createBomb(const Vector2& pos, const Vector2& size) = 0;

    virtual ~AbstractFactory() = default;
};

} // namespace core

#endif // BOMBERMAN_CORE_ABSTRACTFACTORY_H