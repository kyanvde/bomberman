#ifndef BOMBERMAN_GAME_CONCRETEFACTORY_H
#define BOMBERMAN_GAME_CONCRETEFACTORY_H

#include "AbstractFactory.h"
#include "EntityModel.h"
#include "Vector2.h"
#include "views/GrassView.h"

#include <string>

/**
 * @brief The ConcreteFactory class is a concrete implementation of the core::AbstractFactory interface.
 * It provides methods to create various game entities, such as characters, walls, grass, power-ups, and bombs.
 * The factory uses a spritesheet to define the visual representation of the entities.
 */
class ConcreteFactory final : public core::AbstractFactory {
    /**
     * @brief The path to the spritesheet image file used for rendering the entities.
     */
    std::string spriteSheetPath;

    /**
     * @brief The size of each cell in the spritesheet, representing the dimensions of individual sprites.
     */
    core::Vector2 cellSize;

    /**
     * @brief The margin around the sprites in the spritesheet, used to calculate the correct sprite frame.
     */
    core::Vector2 margin;

    /**
     * @brief The spacing between sprites in the spritesheet, used to calculate the correct sprite frame.
     */
    core::Vector2 spacing;

    /**
     * @brief Retrieves the sprite frame for a given cell in the spritesheet.
     * @param cell The cell coordinates (x, y) in the spritesheet.
     * @return The corresponding core::SpriteFrame for the specified cell.
     */
    [[nodiscard]] core::SpriteFrame frameFor(const core::Vector2& cell) const;

 public:
    /**
     * @brief Constructs a new ConcreteFactory object with the specified spritesheet path, cell size, margin, and spacing.
     * @param spritesheetPath The path to the spritesheet image file.
     * @param cellSize The size of each cell in the spritesheet.
     * @param margin The margin around the sprites in the spritesheet (default is (0, 0)).
     * @param spacing The spacing between sprites in the spritesheet (default is (0, 0)).
     */
    [[nodiscard]] ConcreteFactory(std::string  spritesheetPath, const core::Vector2& cellSize, const core::Vector2& margin = core::Vector2(), const core::Vector2& spacing = core::Vector2());

    /**
     * @brief Creates a character entity with the specified position and size.
     * @param position The position of the character in world coordinates.
     * @param size The size of the character in world coordinates.
     * @return A unique pointer to the created core::EntityModel representing the character.
     */
    std::unique_ptr<core::EntityModel> createCharacter(const core::Vector2& position, const core::Vector2& size) override;

    /**
     * @brief Creates a wall entity with the specified position, size, and destructibility.
     * @param position The position of the wall in world coordinates.
     * @param size The size of the wall in world coordinates.
     * @param destructible A boolean indicating whether the wall is destructible (default is false).
     * @return A unique pointer to the created core::EntityModel representing the wall.
     */
    std::unique_ptr<core::EntityModel> createWall(const core::Vector2 &position, const core::Vector2 &size, bool destructible) override;

    /**
     * @brief Creates a grass entity with the specified position and size.
     * @param position The position of the grass in world coordinates.
     * @param size The size of the grass in world coordinates.
     * @return A unique pointer to the created core::EntityModel representing the grass.
     */
    std::unique_ptr<core::EntityModel> createGrass(const core::Vector2& position, const core::Vector2& size) override;

    /**
     * @brief Creates a power-up entity with the specified position and size.
     * @param position The position of the power-up in world coordinates.
     * @param size The size of the power-up in world coordinates.
     * @return A unique pointer to the created core::EntityModel representing the power-up.
     */
    std::unique_ptr<core::EntityModel> createPowerUp(const core::Vector2& position, const core::Vector2& size) override;

    /**
     * @brief Creates a bomb entity with the specified position and size.
     * @param position The position of the bomb in world coordinates.
     * @param size The size of the bomb in world coordinates.
     * @return A unique pointer to the created core::EntityModel representing the bomb.
     */
    std::unique_ptr<core::EntityModel> createBomb(const core::Vector2& position, const core::Vector2& size) override;
};

#endif // BOMBERMAN_GAME_CONCRETEFACTORY_H