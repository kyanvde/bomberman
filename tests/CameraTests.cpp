#include "Camera.h"
#include "TestRunner.h"

void runCameraTests(tests::TestRunner& runner) {
    using core::Camera;
    using core::Vector2;

    const Camera camera(Vector2(800.f, 600.f));

    const Vector2 topLeft = camera.projectPosition(Vector2(-1.f, -1.f));
    runner.checkNear(topLeft.x, 0.f, "Camera projects world (-1,-1) to screen origin x");
    runner.checkNear(topLeft.y, 0.f, "Camera projects world (-1,-1) to screen origin y");

    const Vector2 bottomRight = camera.projectPosition(Vector2(1.f, 1.f));
    runner.checkNear(bottomRight.x, 800.f, "Camera projects world (1,1) to viewport width");
    runner.checkNear(bottomRight.y, 600.f, "Camera projects world (1,1) to viewport height");

    const Vector2 center = camera.projectPosition(Vector2(0.f, 0.f));
    runner.checkNear(center.x, 400.f, "Camera projects world origin to viewport center x");
    runner.checkNear(center.y, 300.f, "Camera projects world origin to viewport center y");

    const Vector2 size = camera.projectSize(Vector2(1.f, 0.5f));
    runner.checkNear(size.x, 400.f, "Camera projects full-width world size to half the viewport width");
    runner.checkNear(size.y, 150.f, "Camera projects world size height correctly");
}
