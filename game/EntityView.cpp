#include "EntityView.h"

namespace game {

EntityView::EntityView(const core::EntityModel& model, core::SpriteFrame sprite)
    : model(model), position(model.getPosition()), size(model.getSize()), sprite(std::move(sprite)) {}

void EntityView::update(const core::GameEvent&) {
    position = model.get().getPosition();
    size = model.get().getSize();

    if (const auto it = animations.find(model.get().getAnimationType()); it != animations.end()) {
        it->second.update(core::Stopwatch::getInstance().getDeltaTime());
        sprite = it->second.getCurrentFrame();
    }
}

void EntityView::render(core::AbstractRenderer& renderer) const { renderer.drawSprite(position, size, sprite); }

} // namespace game
