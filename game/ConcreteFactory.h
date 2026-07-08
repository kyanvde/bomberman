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

    core::SpriteFrame frameFor(const core::Vector2& cell) const {
        return core::SpriteFrame(spritesheetPath, cellSize, cell, margin, spacing);
    }

 public:
    ConcreteFactory(
        const std::string& spritesheetPath = "assets/sprites/bomberman.png",
        const core::Vector2& cellSize = core::Vector2(8.f, 8.f),
        const core::Vector2& margin = core::Vector2(),
        const core::Vector2& spacing = core::Vector2())
        : spritesheetPath(spritesheetPath), cellSize(cellSize), margin(margin), spacing(spacing) {}

    std::unique_ptr<core::EntityModel> createCharacter(const core::Vector2& position, const core::Vector2& size) override {
        std::unique_ptr<core::EntityModel> character = std::make_unique<core::Character>(position, size);
        character->attach(std::make_shared<CharacterView>(position, size, frameFor(core::Vector2(2.f, 0.f))));
        return character;
    }

    std::unique_ptr<core::EntityModel> createWall(const core::Vector2& position, const core::Vector2& size, bool destructible = false) override {
        std::unique_ptr<core::EntityModel> wall = std::make_unique<core::Wall>(position, size, destructible);
        const core::Vector2 cell = destructible ? core::Vector2(12.f, 0.f) : core::Vector2(11.f, 0.f);
        wall->attach(std::make_shared<WallView>(position, size, frameFor(cell)));
        return wall;
    }

    std::unique_ptr<core::EntityModel> createPowerUp(const core::Vector2& position, const core::Vector2& size) override {
        std::unique_ptr<core::EntityModel> powerUp = std::make_unique<core::PowerUp>(position, size);
        powerUp->attach(std::make_shared<PowerUpView>(position, size, frameFor(core::Vector2(1.f, 1.f))));
        return powerUp;
    }

    std::unique_ptr<core::EntityModel> createBomb(const core::Vector2& position, const core::Vector2& size) override {
        std::unique_ptr<core::EntityModel> bomb = std::make_unique<core::Bomb>(position, size);
        bomb->attach(std::make_shared<BombView>(position, size, frameFor(core::Vector2(2.f, 1.f))));
        return bomb;
    }
};

#endif // BOMBERMAN_GAME_CONCRETEFACTORY_H