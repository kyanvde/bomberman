#include "Subject.h"

namespace core {

void Subject::attach(const std::shared_ptr<Observer>& observer) {
    if (!observer) {
        return;
    }

    observers.push_back(observer);
    observer->update();
}

void Subject::notify(const GameEvent& event) {
    for (const auto& observer : observers) {
        if (observer) {
            observer->update(event);
        }
    }
}

void Subject::render(AbstractRenderer& renderer) const {
    for (const auto& observer : observers) {
        if (observer) {
            observer->render(renderer);
        }
    }
}
} // namespace core