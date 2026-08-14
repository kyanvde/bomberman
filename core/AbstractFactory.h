#ifndef BOMBERMAN_CORE_ABSTRACTFACTORY_H
#define BOMBERMAN_CORE_ABSTRACTFACTORY_H

#include "EntityModel.h"
#include "Vector2.h"
#include <memory>

namespace core {
enum class CharacterColor;

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
     * @param color The color of the character.
     * @return A unique pointer to the created EntityModel representing the character.
     */
    virtual std::unique_ptr<EntityModel> createCharacter(const Vector2& pos, const Vector2& size,
                                                         const CharacterColor& color) = 0;

    /**
     * @brief Creates a wall entity with the specified position, size, and destructibility.
     * @param pos The position of the wall in world coordinates.
     * @param size The size of the wall in world coordinates.
     * @param destructible A boolean indicating whether the wall is destructible.
     * @return A unique pointer to the created EntityModel representing the wall.
     */
    virtual std::unique_ptr<EntityModel> createWall(const Vector2& pos, const Vector2& size, bool destructible) = 0;

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
     * @brief Creates a bomb entity with the specified position, size, owner, and blast radius.
     * @param pos The position of the bomb in world coordinates.
     * @param size The size of the bomb in world coordinates.
     * @param owner The color of the character that placed this bomb.
     * @param radius The blast radius of this bomb, in tiles, in each direction.
     * @return A unique pointer to the created EntityModel representing the bomb.
     */
    virtual std::unique_ptr<EntityModel> createBomb(const Vector2& pos, const Vector2& size,
                                                    const CharacterColor& owner, int radius) = 0;

    /**
     * @brief Creates an explosion entity (a single affected tile of a blast) with the specified
     * position and size.
     * @param pos The position of the explosion in world coordinates.
     * @param size The size of the explosion in world coordinates.
     * @return A unique pointer to the created EntityModel representing the explosion.
     */
    virtual std::unique_ptr<EntityModel> createExplosion(const Vector2& pos, const Vector2& size) = 0;

    /**
     * @brief Virtual destructor for the AbstractFactory class.
     */
    virtual ~AbstractFactory() = default;
};

} // namespace core

#endif // BOMBERMAN_CORE_ABSTRACTFACTORY_H