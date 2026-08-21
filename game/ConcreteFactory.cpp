#include "ConcreteFactory.h"

#include <entities/Bomb.h>
#include <entities/Explosion.h>
#include <entities/Grass.h>
#include <entities/PowerUp.h>
#include <entities/Wall.h>

#include <entities/Character.h>
#include <utility>
#include <views/BombView.h>
#include <views/CharacterView.h>
#include <views/ExplosionView.h>
#include <views/PowerUpView.h>
#include <views/WallView.h>

#include "CharacterColor.h"

namespace game {

core::SpriteFrame ConcreteFactory::frameFor(const core::Vector2& cell) const {
    return {spriteSheetPath, cellSize, cell, margin, spacing};
}

ConcreteFactory::ConcreteFactory(std::string spritesheetPath, const core::Vector2& cellSize,
                                 const core::Vector2& margin, const core::Vector2& spacing)
    : spriteSheetPath(std::move(spritesheetPath)), cellSize(cellSize), margin(margin), spacing(spacing) {}

std::unique_ptr<core::EntityModel> ConcreteFactory::createCharacter(const core::Vector2& position,
                                                                    const core::Vector2& size,
                                                                    const core::CharacterColor& color) {
    std::unique_ptr<core::EntityModel> character = std::make_unique<core::Character>(position, size, color);

    core::Vector2 colorPosition;
    switch (color) {
    case core::CharacterColor::White:
        colorPosition = core::Vector2(1.f, 1.f);
        break;
    case core::CharacterColor::Blue:
        colorPosition = core::Vector2(4.f, 1.f);
        break;
    case core::CharacterColor::Red:
        colorPosition = core::Vector2(7.f, 1.f);
        break;
    case core::CharacterColor::Black:
        colorPosition = core::Vector2(10.f, 1.f);
        break;
    }

    character->attach(std::make_shared<CharacterView>(
        *character, core::SpriteFrame(spriteSheetPath, core::Vector2(16.f, 24.f), colorPosition, margin, spacing),
        color));
    return character;
}

std::unique_ptr<core::EntityModel> ConcreteFactory::createWall(const core::Vector2& position, const core::Vector2& size,
                                                               const bool destructible) {
    std::unique_ptr<core::EntityModel> wall = std::make_unique<core::Wall>(position, size, destructible);
    const core::Vector2 spriteSheetLocation = destructible ? core::Vector2(1.f, 0.f) : core::Vector2(0.f, 0.f);
    wall->attach(std::make_shared<WallView>(*wall, frameFor(spriteSheetLocation)));
    return wall;
}

std::unique_ptr<core::EntityModel> ConcreteFactory::createGrass(const core::Vector2& position,
                                                                const core::Vector2& size, const bool shaded) {
    std::unique_ptr<core::EntityModel> grass = std::make_unique<core::Grass>(position, size);
    grass->attach(
        std::make_shared<GrassView>(*grass, frameFor(shaded ? core::Vector2(3.f, 0.f) : core::Vector2(2.f, 0.f))));
    return grass;
}

std::unique_ptr<core::EntityModel> ConcreteFactory::createPowerUp(const core::Vector2& position,
                                                                  const core::Vector2& size,
                                                                  const core::PowerUpType type) {
    std::unique_ptr<core::EntityModel> powerUp = std::make_unique<core::PowerUp>(position, size, type);

    core::Vector2 typePosition;
    switch (type) {
    case core::PowerUpType::Fire:
        typePosition = core::Vector2(12.f, 2.f);
        break;
    case core::PowerUpType::ExtraBomb:
        typePosition = core::Vector2(12.f, 0.f);
        break;
    case core::PowerUpType::Skates:
        typePosition = core::Vector2(12.f, 4.f);
        break;
    }

    powerUp->attach(std::make_shared<PowerUpView>(*powerUp, frameFor(typePosition)));
    return powerUp;
}

std::unique_ptr<core::EntityModel> ConcreteFactory::createBomb(const core::Vector2& position, const core::Vector2& size,
                                                               const core::CharacterColor& owner, const int radius) {
    std::unique_ptr<core::EntityModel> bomb = std::make_unique<core::Bomb>(position, size, owner, radius);
    bomb->attach(std::make_shared<BombView>(*bomb, frameFor(core::Vector2(6.f, 0.f))));
    return bomb;
}

std::unique_ptr<core::EntityModel> ConcreteFactory::createExplosion(const core::Vector2& position,
                                                                    const core::Vector2& size) {
    std::unique_ptr<core::EntityModel> explosion = std::make_unique<core::Explosion>(position, size);
    explosion->attach(std::make_shared<ExplosionView>(*explosion, frameFor(core::Vector2(7.f, 0.f))));
    return explosion;
}

} // namespace game