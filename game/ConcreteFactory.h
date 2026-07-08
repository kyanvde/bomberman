#ifndef BOMBERMAN_GAME_CONCRETEFACTORY_H
#define BOMBERMAN_GAME_CONCRETEFACTORY_H

#include "AbstractFactory.h"
#include "EntityModel.h"
#include "entities/Bomb.h"
#include "entities/Character.h"
#include "entities/PowerUp.h"
#include "entities/Wall.h"
#include "Vector2.h"
#include "views/BombView.h"
#include "views/CharacterView.h"
#include "views/PowerUpView.h"
#include "views/WallView.h"

#include <string>

class ConcreteFactory : public core::AbstractFactory {
private:
    std::string spritesheetPath;
    core::Vector2 cellSize;
    core::Vector2 margin;
    core::Vector2 spacing;

    core::SpriteFrame frameFor(const core::Vector2& cell) const;

 public:
    ConcreteFactory(const std::string& spritesheetPath, const core::Vector2& cellSize, const core::Vector2& margin = core::Vector2(), const core::Vector2& spacing = core::Vector2());

    std::unique_ptr<core::EntityModel> createCharacter(const core::Vector2& position, const core::Vector2& size) override;

    std::unique_ptr<core::EntityModel> createWall(const core::Vector2& position, const core::Vector2& size, bool destructible = false) override;

    std::unique_ptr<core::EntityModel> createPowerUp(const core::Vector2& position, const core::Vector2& size) override;

    std::unique_ptr<core::EntityModel> createBomb(const core::Vector2& position, const core::Vector2& size) override;
};

#endif // BOMBERMAN_GAME_CONCRETEFACTORY_H