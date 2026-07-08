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

    virtual void attach(std::shared_ptr<Observer>& observer) {
        observers.push_back(observer);
    }
    
    virtual void notify() {
        for (const auto& observer : observers) {
            if (observer) {
                observer->update();
            }
        }
    }
};

} // namespace core

#endif // BOMBERMAN_CORE_SUBJECT_H