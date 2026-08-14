#ifndef BOMBERMAN_GAME_ENTITYVIEW_H
#define BOMBERMAN_GAME_ENTITYVIEW_H

#include "AbstractRenderer.h"
#include "EntityModel.h"
#include "Observer.h"

#include <functional>
#include <iostream>
#include <utility>

#include "Stopwatch.h"

namespace game {

/**
 * @brief The EntityView class represents the visual representation of an entity in the game.
 * It observes changes in the associated EntityModel and updates its position and size accordingly.
 * It also provides a method to render the entity using a specified renderer.
 */
class EntityView : public core::Observer {
protected:
    /**
     * @brief A reference wrapper to the associated EntityModel that this view represents.
     */
    std::reference_wrapper<const core::EntityModel> model;

    /**
     * @brief The position of the entity in world coordinates.
     */
    core::Vector2 position;

    /**
     * @brief The size of the entity in world coordinates.
     */
    core::Vector2 size;

    /**
     * @brief The sprite frame used for rendering the entity.
     */
    core::SpriteFrame sprite;

    /**
     * @brief A key-value store of all the available animations.
     */
    std::unordered_map<core::AnimationType, core::Animation> animations;

public:
    /**
     * @brief Constructs a new EntityView object with the specified EntityModel and sprite frame.
     * @param model A reference to the EntityModel that this view represents.
     * @param sprite The sprite frame used for rendering the entity.
     */
    EntityView(const core::EntityModel& model, core::SpriteFrame sprite)
        : model(model), position(model.getPosition()), size(model.getSize()), sprite(std::move(sprite)) {}

    /**
     * @brief Updates the position and size of the entity view based on the associated EntityModel.
     * This method is called when the observed EntityModel changes.
     */
    void update() override {
        position = model.get().getPosition();
        size = model.get().getSize();

        if (const auto it = animations.find(model.get().getAnimationType()); it != animations.end()) {
            it->second.update(core::Stopwatch::getInstance().getDeltaTime());
            sprite = it->second.getCurrentFrame();
        }
    }

    /**
     * @brief Renders the entity view using the specified renderer.
     * @param renderer The renderer used to draw the entity on the screen.
     */
    void render(core::AbstractRenderer& renderer) const override { renderer.drawSprite(position, size, sprite); }
};

} // namespace game

#endif // BOMBERMAN_GAME_ENTITYVIEW_H