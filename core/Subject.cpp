#include "Subject.h"

namespace core {

    void Subject::attach(const std::shared_ptr<Observer>& observer) {
        observers.push_back(observer);
    }

    void Subject::notify() {
        for (const auto& observer : observers) {
            if (observer) {
                observer->update();
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
}