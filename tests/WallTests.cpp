#include "TestRunner.h"
#include "entities/Wall.h"

void runWallTests(tests::TestRunner& runner) {
    using core::Vector2;
    using core::Wall;

    const Wall destructibleWall(Vector2(-1.f, -1.f), Vector2(0.1f, 0.1f), true);
    runner.check(destructibleWall.isDestructible(), "A wall constructed as destructible reports isDestructible()");

    const Wall solidWall(Vector2(-1.f, -1.f), Vector2(0.1f, 0.1f), false);
    runner.check(!solidWall.isDestructible(), "A wall constructed as indestructible reports !isDestructible()");

    const Wall defaultWall(Vector2(-1.f, -1.f), Vector2(0.1f, 0.1f));
    runner.check(!defaultWall.isDestructible(), "A wall's destructible flag defaults to false");
}
