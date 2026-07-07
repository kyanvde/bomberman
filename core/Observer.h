#ifndef BOMBERMAN_CORE_OBSERVER_H
#define BOMBERMAN_CORE_OBSERVER_H

namespace core {

class Observer {
public:
    virtual ~Observer() = default;
    virtual void update() = 0;
};

} // namespace core

#endif // BOMBERMAN_CORE_OBSERVER_H