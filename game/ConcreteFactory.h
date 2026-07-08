#ifndef BOMBERMAN_GAME_CONCRETEFACTORY_H
#define BOMBERMAN_GAME_CONCRETEFACTORY_H

#include "AbstractFactory.h"
#include "EntityModel.h"
#include "entities/Bomb.h"
#include "entities/Character.h"
#include "entities/PowerUp.h"
#include "entities/Wall.h"
#include "Vector2.h"

class ConcreteFactory : public core::AbstractFactory {
 public:
    std::unique_ptr<core::EntityModel> createCharacter(const core::Vector2& position, const core::Vector2& size) override {
        return std::make_unique<core::Character>(position, size);
    }

    std::unique_ptr<core::EntityModel> createWall(const core::Vector2& position, const core::Vector2& size, bool destructible = false) override {
        return std::make_unique<core::Wall>(position, size);
    }

    std::unique_ptr<core::EntityModel> createPowerUp(const core::Vector2& position, const core::Vector2& size) override {
        return std::make_unique<core::PowerUp>(position, size);
    }

    std::unique_ptr<core::EntityModel> createBomb(const core::Vector2& position, const core::Vector2& size) override {
        return std::make_unique<core::Bomb>(position, size);
    }
};

#endif // BOMBERMAN_GAME_CONCRETEFACTORY_H