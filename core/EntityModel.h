#ifndef BOMBERMAN_CORE_ENTITYMODEL_H
#define BOMBERMAN_CORE_ENTITYMODEL_H

#include "Subject.h"
#include "Vector2.h"

namespace core {

class EntityModel : public Subject {
protected:
    Vector2 position;
    Vector2 size;
public:
    EntityModel(const Vector2& pos, const Vector2& size) : position(pos), size(size) {}
};

} // namespace core

#endif // BOMBERMAN_CORE_ENTITYMODEL_H