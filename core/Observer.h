#ifndef BOMBERMAN_CORE_OBSERVER_H
#define BOMBERMAN_CORE_OBSERVER_H

#include "Renderer.h"

namespace core {

/**
 * @brief The Observer class represents the observer in the Observer design pattern.
 * It defines the interface for objects that want to be notified of changes in a subject.
 */
class Observer {
public:
    /**
     * @brief Virtual destructor for the Observer class.
     */
    virtual ~Observer() = default;

    /**
     * @brief Updates the observer based on changes in the subject.
     * This method should be implemented by derived classes to define how they respond to changes.
     */
    virtual void update() = 0;

    /**
     * @brief Renders the observer using the provided renderer.
     * This method should be implemented by derived classes to define how they are rendered.
     * @param renderer A reference to the Renderer used for rendering.
     */
    virtual void render(Renderer& renderer) const = 0;
};

} // namespace core

#endif // BOMBERMAN_CORE_OBSERVER_H