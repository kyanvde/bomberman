#ifndef BOMBERMAN_CORE_OBSERVER_H
#define BOMBERMAN_CORE_OBSERVER_H

#include "AbstractRenderer.h"
#include "GameEvent.h"

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
     * @param event What happened and who it's attributed to. Defaults to a routine Tick, so a
     * plain notify()/update() call (position or animation changes, unrelated to scoring) keeps
     * working exactly as before.
     */
    virtual void update(const GameEvent& event = GameEvent{}) = 0;

    /**
     * @brief Renders the observer using the provided renderer. Default implementation does
     * nothing -- most Observers are visual (EntityView and friends) and override this, but a
     * non-visual Observer like Score has no sprite to draw and shouldn't be forced to provide a
     * meaningless override just to satisfy the interface.
     * @param renderer A reference to the Renderer used for rendering.
     */
    virtual void render(AbstractRenderer&) const {}
};

} // namespace core

#endif // BOMBERMAN_CORE_OBSERVER_H