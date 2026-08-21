#include "Stopwatch.h"

namespace core {

Stopwatch::Stopwatch() noexcept = default;

Stopwatch& Stopwatch::getInstance() noexcept {
    static Stopwatch instance;
    return instance;
}

void Stopwatch::tick() noexcept {
    const std::chrono::high_resolution_clock::time_point currentTime = std::chrono::high_resolution_clock::now();
    const std::chrono::duration<float> elapsedTime = currentTime - lastTime;
    deltaTime = elapsedTime.count();
    lastTime = currentTime;
}

float Stopwatch::getDeltaTime() const noexcept { return deltaTime; }

} // namespace core