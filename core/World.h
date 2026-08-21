#ifndef BOMBERMAN_CORE_WORLD_H
#define BOMBERMAN_CORE_WORLD_H

#include "AbstractFactory.h"
#include "AbstractRenderer.h"
#include "EntityModel.h"
#include "GameOutcome.h"
#include "TileGrid.h"
#include <cstddef>
#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace core {

enum class CharacterColor;
class Score;

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
     * @brief The current round's Score, if one was supplied at construction. Attached as an
     * Observer to every entity added to this world (including ones loaded from the world file),
     * so it can react to GameEvents fired by World as they happen.
     */
    std::shared_ptr<Score> score;

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
     * @brief Identifiers of entities marked for removal, erased from entities at the end of the
     * current update() call rather than immediately, so in-progress iteration over entities is
     * never invalidated mid-tick.
     */
    std::vector<EntityId> pendingRemoval;

    /**
     * @brief Erases every entity currently marked for removal from entities and clears
     * pendingRemoval. Called once at the end of update().
     */
    void flushPendingRemovals();

    /**
     * @brief Checks whether the entity with the given identifier is already queued for removal
     * this tick. Used as a chain-reaction reentrancy guard: a bomb already mid-detonation (queued
     * for removal) should not have its explosion propagation logic run a second time if another
     * blast also reaches it in the same cascade.
     * @param entityId The identifier to check.
     * @return True if entityId is currently pending removal, false otherwise.
     */
    [[nodiscard]] bool isPendingRemoval(EntityId entityId) const;

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
     * @brief Works out how far along one axis a mover may actually travel this tick.
     *
     * Taking either the whole step or none of it leaves a character stranded up to a full frame's
     * travel short of whatever it is walking into, because the moment the next step would overlap,
     * the entire step is refused. That gap is bigger than a tile boundary is forgiving: characters
     * are exactly one tile in size, so a character that can never quite close the last fraction of
     * a step is left permanently straddling two tiles, unable to turn and -- for a bot fleeing a
     * blast -- unable to get its trailing half out of the danger. Clamping to the contact point
     * instead lets it finish flush against the obstacle.
     * @param moverIndex The index of the moving entity in the entities vector.
     * @param position The mover's current position.
     * @param size The mover's size.
     * @param horizontal True to resolve along x, false along y.
     * @param requested The distance the mover would travel if nothing were in the way.
     * @return The largest portion of requested that keeps the mover clear.
     */
    [[nodiscard]] float resolveAxisStep(std::size_t moverIndex, const Vector2& position, const Vector2& size,
                                        bool horizontal, float requested) const;

    /**
     * @brief Works out a "corner assist" nudge for a character whose axis-aligned move was
     * blocked. Characters are exactly one tile in size, so once a character sits even slightly
     * off-grid its box straddles two columns (or rows) and any perpendicular turn collides with
     * whichever neighbour holds a wall -- which, with continuous movement, is almost always.
     * Rather than requiring pixel-perfect alignment from the player, this slides the character
     * toward the tile it is already mostly inside, but only when doing so would genuinely open up
     * the blocked move.
     * @param moverIndex The index of the moving entity in the entities vector.
     * @param position The character's current position.
     * @param size The character's size.
     * @param direction The normalized direction the character is trying to move in.
     * @param maxStep The furthest the character may travel this tick.
     * @return The nudged position, or std::nullopt if no assist applies.
     */
    [[nodiscard]] std::optional<Vector2> cornerAssistPosition(std::size_t moverIndex, const Vector2& position,
                                                              const Vector2& size, const Vector2& direction,
                                                              float maxStep) const;

    /**
     * @brief Spawns a bomb centered on the tile occupied by the entity at the given vector index,
     * with the given blast radius and owner.
     * @param entityIndex The index of the entity (player or AI) placing the bomb.
     * @param radius The blast radius of the new bomb, in tiles, in each direction.
     * @param owner The color of the character placing the bomb.
     */
    void spawnBombAtIndex(std::size_t entityIndex, int radius, const CharacterColor& owner);

    /**
     * @brief Checks whether any entity overlapping the given tile blocks an explosion from
     * propagating past it and is not itself destructible (i.e. an indestructible wall).
     * @param tilePosition The top-left corner of the tile to check.
     * @param tileSize The size of the tile to check.
     * @return True if the blast cannot reach this tile at all, false otherwise.
     */
    [[nodiscard]] bool isBlastStoppedAt(const Vector2& tilePosition, const Vector2& tileSize) const;

    /**
     * @brief Applies an explosion's effect to a single tile: destroys any destructible wall
     * there, kills any entity that dies from explosions, destroys any power-up there, and
     * chain-detonates any bomb there. Fires BlockDestroyed/EntityKilled GameEvents attributed to
     * the bomb's owner.
     * @param tilePosition The top-left corner of the tile to affect.
     * @param tileSize The size of the tile to affect.
     * @param owner The color of the character that placed the bomb causing this explosion, for
     * event attribution.
     * @return True if a destructible wall was destroyed on this tile (signals the caller that
     * the blast should not propagate further in this direction), false otherwise.
     */
    bool explodeTile(const Vector2& tilePosition, const Vector2& tileSize, const CharacterColor& owner);

    /**
     * @brief Re-shades the grass tile directly below a wall that has just been destroyed, if one
     * exists. A grass tile's shading (whether it looks like it's in a wall's shadow) is decided
     * once, when the grass is created, from whatever occupies the tile above it at that moment.
     * That snapshot goes stale the instant the wall above is destroyed -- the grass keeps looking
     * shaded even though nothing is casting a shadow on it any more -- so the tile below a
     * destroyed wall needs to be recreated with its shading reassessed.
     * @param wallPosition The top-left corner of the tile the destroyed wall occupied.
     * @param wallSize The size of that tile.
     */
    void refreshGrassShadingBelow(const Vector2& wallPosition, const Vector2& wallSize);

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
    [[nodiscard]] std::optional<EntityId> getPlayerId() const noexcept;

    /**
     * @brief Checks whether an entity with the given identifier currently exists in the world.
     * @param entityId The identifier to look up.
     * @return True if such an entity exists, false otherwise.
     */
    [[nodiscard]] bool hasEntity(EntityId entityId) const;

    /**
     * @brief Marks the entity with the given identifier for removal. The entity is not erased
     * immediately; it is removed at the end of the current update() call. Marking an id that
     * does not exist, or that is already marked, has no additional effect.
     * @param entityId The identifier of the entity to remove.
     */
    void markForRemoval(EntityId entityId);

    /**
     * @brief Advances the world by one simulation tick: lets every entity present at the start of
     * the tick perform its own time-based self-maintenance (EntityModel::onTick), refreshes every
     * entity's view, then removes any entities that were marked for removal during this tick.
     * Entities created during this tick (e.g. by an explosion) are picked up on the next tick.
     * @param deltaTime The time elapsed since the previous tick, in seconds.
     */
    void update(float deltaTime);

    /**
     * @brief Renders the world and its entities using the provided Renderer.
     * @param renderer The Renderer used to draw the world and entities.
     */
    void render(AbstractRenderer& renderer) const;

    /**
     * @brief Constructs a new World object using the specified factory and loads the world data from the given file.
     * @param factory A shared pointer to an AbstractFactory used for creating game entities.
     * @param filename The path to the file containing the world data.
     * @param score The current round's Score, or nullptr if none (e.g. in tests). Stored before
     * loading, so entities created while loading the world file are covered too.
     */
    World(const std::shared_ptr<AbstractFactory>& factory, const std::string& filename,
          const std::shared_ptr<Score>& score = nullptr);

    /**
     * @brief Sets the world's tile size. Called once by WorldLoader after parsing the world file.
     * @param size The size of a single tile in world coordinates.
     */
    void setCellSize(const Vector2& size);

    /**
     * @brief Spawns a bomb centered on the tile occupied by the character with the given
     * identifier, using that character's current bomb radius and color, provided the character
     * currently has a free bomb slot. Does nothing if no entity with that identifier exists, or
     * if it cannot currently place another bomb.
     * @param characterId The identifier of the character (player or AI) placing the bomb.
     */
    void placeBomb(EntityId characterId);

    /**
     * @brief Checks whether a character of the given color currently overlaps the given tile.
     * Used by a bomb to determine whether its owner has left its tile yet.
     * @param tilePosition The top-left corner of the tile to check.
     * @param tileSize The size of the tile to check.
     * @param color The character color to look for.
     * @return True if a character of that color overlaps the tile, false otherwise.
     */
    [[nodiscard]] bool isTileOccupiedByColor(const Vector2& tilePosition, const Vector2& tileSize,
                                             const CharacterColor& color) const;

    /**
     * @brief Checks whether any wall (destructible or indestructible) currently overlaps the
     * given tile. Used both while loading a world (to decide whether a grass tile should be
     * shaded, i.e. whether the tile above it is a wall) and when a destructible wall is destroyed
     * by an explosion (to shade the grass tile revealed underneath it the same way).
     * @param tilePosition The top-left corner of the tile to check.
     * @param tileSize The size of the tile to check.
     * @return True if a wall overlaps the tile, false otherwise.
     */
    [[nodiscard]] bool isWallAt(const Vector2& tilePosition, const Vector2& tileSize) const;

    /**
     * @brief Checks whether grass currently overlaps the given tile.
     * @param tilePosition The top-left corner of the tile to check.
     * @param tileSize The size of the tile to check.
     * @return True if grass overlaps the tile, false otherwise.
     */
    [[nodiscard]] bool hasGrassAt(const Vector2& tilePosition, const Vector2& tileSize) const;

    /**
     * @brief Checks whether a power-up currently overlaps the given tile.
     * @param tilePosition The top-left corner of the tile to check.
     * @param tileSize The size of the tile to check.
     * @return True if a power-up overlaps the tile, false otherwise.
     */
    [[nodiscard]] bool hasPowerUpAt(const Vector2& tilePosition, const Vector2& tileSize) const;

    /**
     * @brief Retrieves the world's tile size.
     * @return The size of a single grid tile in world coordinates.
     */
    [[nodiscard]] const Vector2& getCellSize() const noexcept;

    /**
     * @brief Retrieves the number of tile columns in the world grid.
     */
    [[nodiscard]] int getGridWidth() const noexcept;

    /**
     * @brief Retrieves the number of tile rows in the world grid.
     */
    [[nodiscard]] int getGridHeight() const noexcept;

    /**
     * @brief Converts a world position to the integer coordinates of the tile it falls within,
     * based on the entity's center point.
     * @param position The entity's current (possibly mid-tile) position.
     * @param size The entity's size, used to find its center.
     * @return The tile coordinates the entity's center falls within.
     */
    [[nodiscard]] GridCoord toGrid(const Vector2& position, const Vector2& size) const;

    /**
     * @brief Converts tile coordinates back to the world position of that tile's top-left corner.
     * @param coord The tile coordinates to convert.
     * @return The tile's top-left position in world coordinates.
     */
    [[nodiscard]] Vector2 toWorldPosition(const GridCoord& coord) const;

    /**
     * @brief Lists every tile the given rectangle overlaps (up to four, for a tile-sized entity
     * straddling a corner).
     *
     * Explosions catch anything whose rectangle overlaps a blast tile, not merely whatever is
     * centred on one -- so a character that has only just crossed into a safe tile is still, in
     * practice, half inside the one it came from. Navigation code needs this to tell whether a
     * character is genuinely clear of a blast rather than only nominally standing on a safe tile.
     * @param position The rectangle's top-left corner.
     * @param size The rectangle's size.
     * @return The coordinates of every tile the rectangle overlaps.
     */
    [[nodiscard]] std::vector<GridCoord> tilesOverlapping(const Vector2& position, const Vector2& size) const;

    /**
     * @brief Builds a tile-indexed snapshot of everything an AI controller needs in order to
     * navigate the world: blocked tiles, things worth reaching, and tiles inside a live blast.
     * Filled in a single pass over the entities, so a controller can run a full search over the
     * map without re-querying the world per tile.
     * @return The snapshot, valid only for the moment it was taken.
     */
    [[nodiscard]] TileGrid buildTileGrid() const;

    /**
     * @brief Notifies whichever character has the given color that one of its bombs has just
     * exploded (or otherwise been removed), freeing up a bomb slot. Does nothing if no character
     * of that color exists.
     * @param owner The color of the character to notify.
     */
    void notifyBombExploded(const CharacterColor& owner);

    /**
     * @brief Detonates the bomb with the given identifier: removes it, frees its owner's bomb
     * slot, and propagates a cross-shaped blast in all four directions up to the given radius.
     * Each direction stops at the first indestructible wall (which the blast never reaches), or
     * immediately after destroying the first destructible wall it reaches. Along the way, any
     * killable entity dies, any power-up is destroyed, and any bomb caught in the blast
     * chain-detonates in turn. Safe to call more than once for the same bomb within a single
     * chain reaction: a bomb already mid-detonation is not processed twice.
     * @param bombId The identifier of the bomb entity to detonate.
     * @param radius The blast radius, in tiles, in each direction.
     * @param owner The color of the character that placed this bomb.
     */
    void detonateBomb(EntityId bombId, int radius, const CharacterColor& owner);

    /**
     * @brief Reports the current result of the round: the Player has lost if the White character
     * exists and is dead (checked first, so a simultaneous Player/last-bot death resolves as a
     * loss); otherwise the Player has won if at least one bot character existed and all of them
     * are dead; otherwise the round is still in progress.
     * @return The current GameOutcome.
     */
    [[nodiscard]] GameOutcome getOutcome() const;
};

} // namespace core

#endif // BOMBERMAN_CORE_WORLD_H