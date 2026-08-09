#ifndef BOMBERMAN_CORE_ABSTRACTFACTORY_H
#define BOMBERMAN_CORE_ABSTRACTFACTORY_H

#include <memory>
#include "EntityModel.h"
#include "Vector2.h"

namespace core {

/**
 * @brief The AbstractFactory class defines the interface for creating various game entities.
 * It provides pure virtual methods for creating characters, walls, grass, power-ups, and bombs.
 * Concrete implementations of this factory will provide specific creation logic for each entity type.
 */
class AbstractFactory {
public:
    /**
     * @brief Creates a character entity with the specified position and size.
     * @param pos The position of the character in world coordinates.
     * @param size The size of the character in world coordinates.
     * @return A unique pointer to the created EntityModel representing the character.
     */
    virtual std::unique_ptr<EntityModel> createCharacter(const Vector2& pos, const Vector2& size) = 0;

    /**
     * @brief Creates a wall entity with the specified position, size, and destructibility.
     * @param pos The position of the wall in world coordinates.
     * @param size The size of the wall in world coordinates.
     * @param destructible A boolean indicating whether the wall is destructible.
     * @return A unique pointer to the created EntityModel representing the wall.
     */
    virtual std::unique_ptr<EntityModel> createWall(const Vector2 &pos, const Vector2 &size, bool destructible) = 0;

    /**
     * @brief Creates a grass entity with the specified position and size.
     * @param pos The position of the grass in world coordinates.
     * @param size The size of the grass in world coordinates.
     * @param shaded A boolean indicating whether the grass is shaded.
     * @return A unique pointer to the created EntityModel representing the grass.
     */
    virtual std::unique_ptr<EntityModel> createGrass(const Vector2& pos, const Vector2& size, bool shaded) = 0;

    /**
     * @brief Creates a power-up entity with the specified position and size.
     * @param pos The position of the power-up in world coordinates.
     * @param size The size of the power-up in world coordinates.
     * @return A unique pointer to the created EntityModel representing the power-up.
     */
    virtual std::unique_ptr<EntityModel> createPowerUp(const Vector2& pos, const Vector2& size) = 0;

    /**
     * @brief Creates a bomb entity with the specified position and size.
     * @param pos The position of the bomb in world coordinates.
     * @param size The size of the bomb in world coordinates.
     * @return A unique pointer to the created EntityModel representing the bomb.
     */
    virtual std::unique_ptr<EntityModel> createBomb(const Vector2& pos, const Vector2& size) = 0;

    /**
     * @brief Virtual destructor for the AbstractFactory class.
     */
    virtual ~AbstractFactory() = default;
};

} // namespace core

#endif // BOMBERMAN_CORE_ABSTRACTFACTORY_H