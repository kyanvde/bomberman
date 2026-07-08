#include "World.h"

#include "WorldLoader.h"
#include <iostream>

namespace core {
    void World::addEntity(std::unique_ptr<EntityModel> entity) {
        entities.push_back(std::move(entity));
        std::cout << "Entity added. Total entities: " << entities.size() << std::endl;
    }

    void World::render(Renderer& renderer) const {
        for (const auto& entity : entities) {
            if (entity) {
                entity->render(renderer);
            }
        }
    }

    World::World(std::shared_ptr<AbstractFactory> factory, const std::string& filename) : factory(factory) {
        WorldLoader::loadFromFile(filename, *this, factory);
    }
} // namespace core