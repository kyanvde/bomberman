#include "Camera.h"
#include "TestRunner.h"

void runCameraTests(tests::TestRunner& runner) {
    using core::Camera;
    using core::Vector2;

    // A non-square 800x600 viewport: the world's [-1,1] square must scale uniformly on both axes
    // (using the smaller dimension, 600) and be centered in the wider one, rather than being
    // stretched to fill 800x600 -- otherwise every sprite would be squashed by a different amount
    // depending on the window's current aspect ratio.
    const Camera camera(Vector2(800.f, 600.f));

    const Vector2 topLeft = camera.projectPosition(Vector2(-1.f, -1.f));
    runner.checkNear(topLeft.x, 100.f, "Camera pillarboxes world (-1,-1) to the left edge of the centered square");
    runner.checkNear(topLeft.y, 0.f, "Camera projects world (-1,-1) to the viewport's top edge");

    const Vector2 bottomRight = camera.projectPosition(Vector2(1.f, 1.f));
    runner.checkNear(bottomRight.x, 700.f, "Camera pillarboxes world (1,1) to the right edge of the centered square");
    runner.checkNear(bottomRight.y, 600.f, "Camera projects world (1,1) to the viewport's bottom edge");

    const Vector2 center = camera.projectPosition(Vector2(0.f, 0.f));
    runner.checkNear(center.x, 400.f, "Camera projects world origin to viewport center x");
    runner.checkNear(center.y, 300.f, "Camera projects world origin to viewport center y");

    const Vector2 size = camera.projectSize(Vector2(1.f, 0.5f));
    runner.checkNear(size.x, 300.f, "Camera scales world size x uniformly, using the viewport's smaller dimension");
    runner.checkNear(size.y, 150.f, "Camera scales world size y by the same uniform factor as x");

    // A square viewport has no letterbox/pillarbox offset at all: the two axes were already
    // consistent, so this must reduce to the same behavior as a naive per-axis projection.
    const Camera squareCamera(Vector2(500.f, 500.f));
    const Vector2 squareTopLeft = squareCamera.projectPosition(Vector2(-1.f, -1.f));
    runner.checkNear(squareTopLeft.x, 0.f, "A square viewport has no pillarboxing on x");
    runner.checkNear(squareTopLeft.y, 0.f, "A square viewport has no letterboxing on y");
}
