#include "TestRunner.h"
#include "Vector2.h"

void runVector2Tests(tests::TestRunner& runner) {
    using core::Vector2;

    const Vector2 a(1.f, 2.f);
    const Vector2 b(3.f, -1.f);

    const Vector2 sum = a + b;
    runner.checkNear(sum.x, 4.f, "Vector2 operator+ x component");
    runner.checkNear(sum.y, 1.f, "Vector2 operator+ y component");

    const Vector2 diff = a - b;
    runner.checkNear(diff.x, -2.f, "Vector2 operator- x component");
    runner.checkNear(diff.y, 3.f, "Vector2 operator- y component");

    const Vector2 scaled = a * 2;
    runner.checkNear(scaled.x, 2.f, "Vector2 operator* x component");
    runner.checkNear(scaled.y, 4.f, "Vector2 operator* y component");

    const Vector2 defaulted;
    runner.checkNear(defaulted.x, 0.f, "Vector2 default constructor initializes x to zero");
    runner.checkNear(defaulted.y, 0.f, "Vector2 default constructor initializes y to zero");
}
