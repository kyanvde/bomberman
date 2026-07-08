#ifndef BOMBERMAN_CORE_WORLD_H
#define BOMBERMAN_CORE_WORLD_H

#include <memory>
#include <string>
#include <vector>
#include "AbstractFactory.h"
#include "EntityModel.h"
#include "Renderer.h"

namespace core {

class World {
private:
    std::shared_ptr<AbstractFactory> factory;

    std::vector<std::unique_ptr<EntityModel>> entities;
public:
    void addEntity(std::unique_ptr<EntityModel> entity);

    void render(Renderer& renderer) const;

    World(std::shared_ptr<AbstractFactory> factory, const std::string& filename);
};

} // namespace core

#endif // BOMBERMAN_CORE_WORLD_H