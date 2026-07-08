#include "ConcreteFactory.h"

core::SpriteFrame ConcreteFactory::frameFor(const core::Vector2& cell) const {
    return core::SpriteFrame(spritesheetPath, cellSize, cell, margin, spacing);
}

ConcreteFactory::ConcreteFactory(const std::string& spritesheetPath, const core::Vector2& cellSize, const core::Vector2& margin, const core::Vector2& spacing)
    : spritesheetPath(spritesheetPath), cellSize(cellSize), margin(margin), spacing(spacing) {}

std::unique_ptr<core::EntityModel> ConcreteFactory::createCharacter(const core::Vector2& position, const core::Vector2& size) {
    std::unique_ptr<core::EntityModel> character = std::make_unique<core::Character>(position, size);
    character->attach(std::make_shared<CharacterView>(position, size, frameFor(core::Vector2(2.f, 0.f))));
    return character;
}

std::unique_ptr<core::EntityModel> ConcreteFactory::createWall(const core::Vector2& position, const core::Vector2& size, bool destructible = false) {
    std::unique_ptr<core::EntityModel> wall = std::make_unique<core::Wall>(position, size, destructible);
    const core::Vector2 spriteSheetLocation = destructible ? core::Vector2(12.f, 0.f) : core::Vector2(11.f, 0.f);
    wall->attach(std::make_shared<WallView>(position, size, frameFor(spriteSheetLocation)));
    return wall;
}

std::unique_ptr<core::EntityModel> ConcreteFactory::createPowerUp(const core::Vector2& position, const core::Vector2& size) {
    std::unique_ptr<core::EntityModel> powerUp = std::make_unique<core::PowerUp>(position, size);
    powerUp->attach(std::make_shared<PowerUpView>(position, size, frameFor(core::Vector2(1.f, 1.f))));
    return powerUp;
}

std::unique_ptr<core::EntityModel> ConcreteFactory::createBomb(const core::Vector2& position, const core::Vector2& size) {
    std::unique_ptr<core::EntityModel> bomb = std::make_unique<core::Bomb>(position, size);
    bomb->attach(std::make_shared<BombView>(position, size, frameFor(core::Vector2(2.f, 1.f))));
    return bomb;
}