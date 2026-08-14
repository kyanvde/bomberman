#ifndef BOMBERMAN_TESTS_TESTFACTORY_H
#define BOMBERMAN_TESTS_TESTFACTORY_H

#include "AbstractFactory.h"
#include "entities/Bomb.h"
#include "entities/Character.h"
#include "entities/Grass.h"
#include "entities/PowerUp.h"
#include "entities/Wall.h"

namespace tests {

/**
 * @brief A bare-bones core::AbstractFactory for tests: constructs real core entities with no
 * attached observers/views, so core's game logic can be exercised in isolation from the
 * SFML-based representation layer.
 */
class TestFactory final : public core::AbstractFactory {
public:
    std::unique_ptr<core::EntityModel> createCharacter(const core::Vector2& pos, const core::Vector2& size,
                                                       const core::CharacterColor& color) override {
        return std::make_unique<core::Character>(pos, size, color);
    }

    std::unique_ptr<core::EntityModel> createWall(const core::Vector2& pos, const core::Vector2& size,
                                                  const bool destructible) override {
        return std::make_unique<core::Wall>(pos, size, destructible);
    }

    std::unique_ptr<core::EntityModel> createGrass(const core::Vector2& pos, const core::Vector2& size,
                                                   bool /*shaded*/) override {
        return std::make_unique<core::Grass>(pos, size);
    }

    std::unique_ptr<core::EntityModel> createPowerUp(const core::Vector2& pos, const core::Vector2& size) override {
        return std::make_unique<core::PowerUp>(pos, size);
    }

    std::unique_ptr<core::EntityModel> createBomb(const core::Vector2& pos, const core::Vector2& size,
                                                  const core::CharacterColor& owner, const int radius) override {
        return std::make_unique<core::Bomb>(pos, size, owner, radius);
    }
};

} // namespace tests

#endif // BOMBERMAN_TESTS_TESTFACTORY_H
