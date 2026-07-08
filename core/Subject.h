#ifndef BOMBERMAN_CORE_SUBJECT_H
#define BOMBERMAN_CORE_SUBJECT_H

#include <memory>
#include <vector>
#include "Observer.h"

namespace core {

class Subject {
protected:
    std::vector<std::shared_ptr<Observer>> observers;
public:
    virtual ~Subject() = default;

    virtual void attach(const std::shared_ptr<Observer>& observer);
    
    virtual void notify();

    virtual void render(Renderer& renderer) const;
};

} // namespace core

#endif // BOMBERMAN_CORE_SUBJECT_H