#include "ConcreteFactory.h"

#include <entities/Bomb.h>
#include <entities/Grass.h>
#include <entities/PowerUp.h>
#include <entities/Wall.h>

#include <utility>
#include <entities/Character.h>
#include <views/CharacterView.h>
#include <views/WallView.h>
#include <views/PowerUpView.h>
#include <views/BombView.h>

core::SpriteFrame ConcreteFactory::frameFor(const core::Vector2& cell) const {
    return {spriteSheetPath, cellSize, cell, margin, spacing};
}

ConcreteFactory::ConcreteFactory(std::string spritesheetPath, const core::Vector2& cellSize, const core::Vector2& margin, const core::Vector2& spacing)
    : spriteSheetPath(std::move(spritesheetPath)), cellSize(cellSize), margin(margin), spacing(spacing) {}

std::unique_ptr<core::EntityModel> ConcreteFactory::createCharacter(const core::Vector2& position, const core::Vector2& size) {
    std::unique_ptr<core::EntityModel> character = std::make_unique<core::Character>(position, size);
    character->attach(std::make_shared<CharacterView>(
        *character,
        core::SpriteFrame(spriteSheetPath, core::Vector2(16.f, 24.f), core::Vector2(1.f, 1.f), margin, spacing)
        ));
    return character;
}

std::unique_ptr<core::EntityModel> ConcreteFactory::createWall(const core::Vector2& position, const core::Vector2& size, const bool destructible) {
    std::unique_ptr<core::EntityModel> wall = std::make_unique<core::Wall>(position, size, destructible);
    const core::Vector2 spriteSheetLocation = destructible ? core::Vector2(1.f, 0.f) : core::Vector2(0.f, 0.f);
    wall->attach(std::make_shared<WallView>(*wall, frameFor(spriteSheetLocation)));
    return wall;
}

std::unique_ptr<core::EntityModel> ConcreteFactory::createGrass(const core::Vector2& position, const core::Vector2& size) {
    std::unique_ptr<core::EntityModel> grass = std::make_unique<core::Grass>(position, size);
    grass->attach(std::make_shared<GrassView>(*grass, frameFor(core::Vector2(2.f, 0.f))));
    return grass;
}

std::unique_ptr<core::EntityModel> ConcreteFactory::createPowerUp(const core::Vector2& position, const core::Vector2& size) {
    std::unique_ptr<core::EntityModel> powerUp = std::make_unique<core::PowerUp>(position, size);
    powerUp->attach(std::make_shared<PowerUpView>(*powerUp, frameFor(core::Vector2(1.f, 1.f))));
    return powerUp;
}

std::unique_ptr<core::EntityModel> ConcreteFactory::createBomb(const core::Vector2& position, const core::Vector2& size) {
    std::unique_ptr<core::EntityModel> bomb = std::make_unique<core::Bomb>(position, size);
    bomb->attach(std::make_shared<BombView>(*bomb, frameFor(core::Vector2(2.f, 1.f))));
    return bomb;
}