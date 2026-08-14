#ifndef BOMBERMAN_TESTS_TESTENTITIES_H
#define BOMBERMAN_TESTS_TESTENTITIES_H

#include "EntityModel.h"
#include "World.h"

#include <memory>
#include <vector>

namespace tests {

/**
 * @brief A minimal core::EntityModel test double that records how many times onTick was called
 * and the deltaTime of the most recent call. Can optionally spawn a batch of further
 * CountingEntity instances into the World the first time it ticks, to exercise tick-triggered
 * mutation of World's entity storage the same way an exploding bomb will in later phases.
 */
class CountingEntity final : public core::EntityModel {
public:
    int tickCount = 0;
    float lastDeltaTime = 0.f;
    int spawnCountOnFirstTick = 0;
    std::vector<CountingEntity*>* spawnedInto = nullptr;

    CountingEntity(const core::Vector2& pos, const core::Vector2& size) : EntityModel(pos, size) {}

    void onTick(core::World& world, core::EntityId, const float deltaTime) override {
        ++tickCount;
        lastDeltaTime = deltaTime;

        if (spawnCountOnFirstTick > 0 && tickCount == 1) {
            const int toSpawn = spawnCountOnFirstTick;
            spawnCountOnFirstTick = 0;

            for (int i = 0; i < toSpawn; ++i) {
                auto child = std::make_unique<CountingEntity>(position, size);
                if (spawnedInto) {
                    spawnedInto->push_back(child.get());
                }
                world.addEntity(std::move(child));
            }
        }
    }
};

} // namespace tests

#endif // BOMBERMAN_TESTS_TESTENTITIES_H
