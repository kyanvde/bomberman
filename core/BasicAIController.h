#ifndef BOMBERMAN_CORE_BASICAICONTROLLER_H
#define BOMBERMAN_CORE_BASICAICONTROLLER_H

#include "AIController.h"

namespace core {

/**
 * @brief A baseline AIController satisfying the assignment's minimum bot behaviors, in priority
 * order: flee imminent bomb danger, attack a nearby enemy (or any enemy once no destructible
 * walls remain), bomb an adjacent destructible wall and retreat from it, seek a nearby power-up,
 * and otherwise work toward the nearest destructible wall. Constructor parameters let different
 * bot instances behave slightly differently without duplicating this decision logic.
 */
class BasicAIController final : public AIController {
    /**
     * @brief How many tiles away an enemy must be to be worth actively pursuing. Ignored (treated
     * as infinite) once no destructible walls remain, since attacking is then the only thing left
     * to do.
     */
    float aggressionRadiusTiles;

    /**
     * @brief How many tiles away a power-up must be to be worth actively pursuing.
     */
    float powerUpSearchRadiusTiles;

public:
    /**
     * @brief Constructs a new BasicAIController with the given behavior tuning.
     * @param aggressionRadiusTiles How many tiles away an enemy must be to be worth pursuing.
     * @param powerUpSearchRadiusTiles How many tiles away a power-up must be to be worth pursuing.
     */
    explicit BasicAIController(float aggressionRadiusTiles = 4.f, float powerUpSearchRadiusTiles = 6.f);

    [[nodiscard]] Decision decide(const World& world, const EntityModel& self) const override;
};

} // namespace core

#endif // BOMBERMAN_CORE_BASICAICONTROLLER_H
