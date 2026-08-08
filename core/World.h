#ifndef BOMBERMAN_CORE_WORLD_H
#define BOMBERMAN_CORE_WORLD_H

#include <cstddef>
#include <memory>
#include <optional>
#include <string>
#include <vector>
#include "AbstractFactory.h"
#include "EntityModel.h"
#include "Renderer.h"

namespace core {

/**
 * @brief The World class represents the game world and manages the entities within it.
 * It provides methods to add entities, move the player, and render the world.
 */
class World {
    /**
     * @brief A shared pointer to an AbstractFactory used for creating game entities.
     */
    std::shared_ptr<AbstractFactory> factory;

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
    [[nodiscard]] static bool insideWorldBounds(const Vector2& position, const Vector2& size) ;
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
    void render(Renderer& renderer) const;

    /**
     * @brief Constructs a new World object using the specified factory and loads the world data from the given file.
     * @param factory A shared pointer to an AbstractFactory used for creating game entities.
     * @param filename The path to the file containing the world data.
     */
    World(std::shared_ptr<AbstractFactory> factory, const std::string& filename);
};

} // namespace core

#endif // BOMBERMAN_CORE_WORLD_H