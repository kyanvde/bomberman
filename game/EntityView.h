#ifndef BOMBERMAN_GAME_ENTITYVIEW_H
#define BOMBERMAN_GAME_ENTITYVIEW_H

#include "EntityModel.h"
#include "Observer.h"
#include "Renderer.h"

#include <functional>

class EntityView : public core::Observer {
protected:
    std::reference_wrapper<const core::EntityModel> model;
    core::Vector2 position;
    core::Vector2 size;
    core::SpriteFrame sprite;

public:
    EntityView(const core::EntityModel& model, const core::SpriteFrame& sprite)
        : model(model), position(model.getPosition()), size(model.getSize()), sprite(sprite) {}

    void update() override {
        position = model.get().getPosition();
        size = model.get().getSize();
    }

    void render(core::Renderer& renderer) const override {
        renderer.drawSprite(position, size, sprite);
    }
};

#endif // BOMBERMAN_GAME_ENTITYVIEW_H