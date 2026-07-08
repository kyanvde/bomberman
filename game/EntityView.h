#ifndef BOMBERMAN_GAME_ENTITYVIEW_H
#define BOMBERMAN_GAME_ENTITYVIEW_H

#include "Observer.h"
#include "Renderer.h"

class EntityView : public core::Observer {
protected:
    core::Vector2 position;
    core::Vector2 size;
    core::SpriteFrame sprite;

public:
    EntityView(const core::Vector2& position, const core::Vector2& size, const core::SpriteFrame& sprite)
        : position(position), size(size), sprite(sprite) {}

    void update() override {}

    void render(core::Renderer& renderer) const override {
        renderer.drawSprite(position, size, sprite);
    }
};

#endif // BOMBERMAN_GAME_ENTITYVIEW_H