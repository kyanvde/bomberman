#ifndef BOMBERMAN_CORE_SUBJECT_H
#define BOMBERMAN_CORE_SUBJECT_H

#include "Observer.h"
#include <memory>
#include <vector>

namespace core {

/**
 * @brief The Subject class represents the subject in the Observer design pattern.
 * It maintains a list of observers and provides methods to attach observers and notify them of changes.
 */
class Subject {
protected:
    /**
     * @brief A vector of shared pointers to Observer objects that are observing this subject.
     */
    std::vector<std::shared_ptr<Observer>> observers;

public:
    /**
     * @brief Virtual destructor for the Subject class.
     */
    virtual ~Subject() = default;

    /**
     * @brief Attaches an observer to the subject.
     * @param observer A shared pointer to the Observer to be attached.
     */
    virtual void attach(const std::shared_ptr<Observer>& observer);

    /**
     * @brief Notifies all attached observers of a change in the subject.
     */
    virtual void notify();

    /**
     * @brief Renders the subject using the provided renderer.
     * @param renderer A reference to the Renderer used for rendering.
     */
    virtual void render(AbstractRenderer& renderer) const;
};

} // namespace core

#endif // BOMBERMAN_CORE_SUBJECT_H