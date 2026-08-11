#ifndef BOMBERMAN_CORE_WORLD_H
#define BOMBERMAN_CORE_WORLD_H

#include <cstddef>
#include <memory>
#include <optional>
#include <string>
#include <vector>
#include "AbstractFactory.h"
#include "EntityModel.h"
#include "AbstractRenderer.h"

namespace core {

/**
 * @brief The World class represents the game world and manages the entities within it.
 * It provides methods to add entities, move the player, and render the world.
 */
class World {
    /**
     * @brief A vector of unique pointers to EntityModel objects representing the entities in the world.
     */
    std::vector<std::unique_ptr<EntityModel>> entities;

    /**
     * @brief An optional index representing the position of the player entity in the entities vector.
     * If the player entity is not present, this will be std::nullopt.
     */
    std::optional<std::size_t> playerIndex;

    /**
     * @brief The factory used to create new entities.
     */
    const std::shared_ptr<AbstractFactory> factory;

    /**
     * @brief The size of a single grid tile in world coordinates, set once during loading.
     */
    Vector2 cellSize;

    /**
     * @brief Checks if the specified mover entity collides with any blocking entity in the world.
     * @param moverIndex The index of the mover entity in the entities vector.
     * @param position The position of the mover entity.
     * @param size The size of the mover entity.
     * @return True if there is a collision with a blocking entity, false otherwise.
     */
    [[nodiscard]] bool collidesWithBlockingEntity(std::size_t moverIndex, const Vector2& position, const Vector2& size) const;

    /**
     * @brief Checks if the specified position and size are within the world bounds.
     * @param position The position to check.
     * @param size The size to check.
     * @return True if the position and size are within the world bounds, false otherwise.
     */
    [[nodiscard]] static bool insideWorldBounds(const Vector2& position, const Vector2& size);

    /**
     * @brief Snaps a world position to the top-left corner of the tile it falls within,
     * based on the entity's center point.
     * @param position The entity's current (possibly mid-tile) position.
     * @param size The entity's size, used to find its center.
     * @return The top-left position of the tile the entity's center falls within.
     */
    [[nodiscard]] Vector2 snapToTileTopLeft(const Vector2& position, const Vector2& size) const;
public:
    /**
     * @brief Adds a new entity to the world.
     * @param entity A unique pointer to the EntityModel to be added.
     */
    void addEntity(std::unique_ptr<EntityModel> entity);

    /**
     * @brief Moves the player entity in the specified direction, considering collisions and world bounds.
     * @param direction The direction vector to move the player.
     * @param deltaTime The time elapsed since the last update, used for movement calculations.
     */
    void movePlayer(const Vector2& direction, float deltaTime) const;

    /**
     * @brief Renders the world and its entities using the provided Renderer.
     * @param renderer The Renderer used to draw the world and entities.
     */
    void render(AbstractRenderer& renderer) const;

    /**
     * @brief Constructs a new World object using the specified factory and loads the world data from the given file.
     * @param factory A shared pointer to an AbstractFactory used for creating game entities.
     * @param filename The path to the file containing the world data.
     */
    World(const std::shared_ptr<AbstractFactory>& factory, const std::string& filename);

    /**
    * @brief Sets the world's tile size. Called once by WorldLoader after parsing the world file.
    * @param size The size of a single tile in world coordinates.
    */
    void setCellSize(const Vector2& size) { cellSize = size; }

    /**
     * @brief Spawns a bomb centered on the tile occupied by the specified entity.
     * @param entityIndex The index of the entity (player or AI) placing the bomb.
     */
    void spawnBombAt(std::size_t entityIndex);

    /**
     * @Brief Spawns a bomb centered on the tile occupied by the character with the specified color.
     * @param color The color of the character placing the bomb
     */
    void spawnBomb(const CharacterColor& color);
};

} // namespace core

#endif // BOMBERMAN_CORE_WORLD_H