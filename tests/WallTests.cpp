#include "TestRunner.h"
#include "entities/Wall.h"

void runWallTests(tests::TestRunner& runner) {
    using core::Vector2;
    using core::Wall;

    const Wall destructibleWall(Vector2(-1.f, -1.f), Vector2(0.1f, 0.1f), true);
    runner.check(destructibleWall.isDestructible(), "A wall constructed as destructible reports isDestructible()");
    runner.check(destructibleWall.blocksExplosion(), "A destructible wall still blocks explosion propagation");
    runner.check(destructibleWall.isDestructibleByExplosion(), "A destructible wall is destroyed by an explosion");

    const Wall solidWall(Vector2(-1.f, -1.f), Vector2(0.1f, 0.1f), false);
    runner.check(!solidWall.isDestructible(), "A wall constructed as indestructible reports !isDestructible()");
    runner.check(solidWall.blocksExplosion(), "An indestructible wall blocks explosion propagation");
    runner.check(!solidWall.isDestructibleByExplosion(), "An indestructible wall is not destroyed by an explosion");

    const Wall defaultWall(Vector2(-1.f, -1.f), Vector2(0.1f, 0.1f));
    runner.check(!defaultWall.isDestructible(), "A wall's destructible flag defaults to false");
}
