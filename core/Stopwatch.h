#ifndef BOMBERMAN_CORE_STOPWATCH_H
#define BOMBERMAN_CORE_STOPWATCH_H

#include <chrono>

namespace core {

class Stopwatch final {
public:
    [[nodiscard]] static Stopwatch& getInstance() noexcept;

    void tick() noexcept;

    [[nodiscard]] float getDeltaTime() const noexcept;

    Stopwatch(const Stopwatch&) = delete;

    Stopwatch& operator=(const Stopwatch&) = delete;

    Stopwatch(Stopwatch&&) = delete;

    Stopwatch& operator=(Stopwatch&&) = delete;

private:
    Stopwatch() noexcept = default;

    std::chrono::high_resolution_clock::time_point lastTime{std::chrono::high_resolution_clock::now()};

    float deltaTime{0.0f};
};

} // namespace core

#endif // BOMBERMAN_CORE_STOPWATCH_H