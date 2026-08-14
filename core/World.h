#ifndef BOMBERMAN_CORE_WORLD_H
#define BOMBERMAN_CORE_WORLD_H

#include "AbstractFactory.h"
#include "AbstractRenderer.h"
#include "EntityModel.h"
#include <cstddef>
#include <memory>
#include <optional>
#include <string>
#include <vector>

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
     * @brief The identifier of the player entity, if it has been added to the world.
     * If the player entity is not present, this will be std::nullopt.
     */
    std::optional<EntityId> playerId;

    /**
     * @brief The identifier to assign to the next entity added via addEntity, incremented each time.
     */
    EntityId nextEntityId = 0;

    /**
     * @brief The factory used to create new entities.
     */
    const std::shared_ptr<AbstractFactory> factory;

    /**
     * @brief The size of a single grid tile in world coordinates, set once during loading.
     */
    Vector2 cellSize;

    /**
     * @brief Finds the current vector index of the entity with the given identifier.
     * @param entityId The identifier of the entity to look up.
     * @return The entity's index in the entities vector, or std::nullopt if no such entity exists.
     */
    [[nodiscard]] std::optional<std::size_t> indexOf(EntityId entityId) const;

    /**
     * @brief Checks if the specified mover entity collides with any blocking entity in the world.
     * @param moverIndex The index of the mover entity in the entities vector.
     * @param position The position of the mover entity.
     * @param size The size of the mover entity.
     * @return True if there is a collision with a blocking entity, false otherwise.
     */
    [[nodiscard]] bool collidesWithBlockingEntity(std::size_t moverIndex, const Vector2& position,
                                                  const Vector2& size) const;

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

    /**
     * @brief Spawns a bomb centered on the tile occupied by the entity at the given vector index.
     * @param entityIndex The index of the entity (player or AI) placing the bomb.
     */
    void spawnBombAtIndex(std::size_t entityIndex);

public:
    /**
     * @brief Adds a new entity to the world.
     * @param entity A unique pointer to the EntityModel to be added.
     */
    void addEntity(std::unique_ptr<EntityModel> entity);

    /**
     * @brief Moves the character with the given identifier in the specified direction, considering collisions and
     * world bounds. Does nothing if no entity with that identifier exists.
     * @param characterId The identifier of the character to move.
     * @param direction The direction vector to move the character.
     * @param deltaTime The time elapsed since the last update, used for movement calculations.
     */
    void moveCharacter(EntityId characterId, const Vector2& direction, float deltaTime);

    /**
     * @brief Retrieves the identifier of the player entity, if it has been added to the world.
     * @return The player's EntityId, or std::nullopt if no player-controlled entity was loaded.
     */
    [[nodiscard]] std::optional<EntityId> getPlayerId() const noexcept { return playerId; }

    /**
     * @brief Updates all entity views for the current frame.
     */
    void update();

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
     * @brief Spawns a bomb centered on the tile occupied by the character with the given identifier.
     * Does nothing if no entity with that identifier exists.
     * @param characterId The identifier of the character (player or AI) placing the bomb.
     */
    void placeBomb(EntityId characterId);
};

} // namespace core

#endif // BOMBERMAN_CORE_WORLD_H