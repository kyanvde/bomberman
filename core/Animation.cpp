#include "Animation.h"

#include <utility>
#include "AbstractRenderer.h"

core::Animation::Animation(std::vector<SpriteFrame> frames, const float frameDuration, const bool loop)
    : frames(std::move(frames)), frameDuration(frameDuration), loop(loop) {}

void core::Animation::update(const float deltaTime) {
    if (frames.size() <= 1 || frameDuration <= 0.f) {
        return;
    }

    elapsed += deltaTime;

    while (elapsed >= frameDuration) {
        elapsed -= frameDuration;

        if (currentIndex + 1 < frames.size()) {
            ++currentIndex;
        } else if (loop) {
            currentIndex = 0;
        } else {
            elapsed = 0.f;
            break;
        }
    }
}

void core::Animation::reset() {
    elapsed = 0.f;
    currentIndex = 0;
}

const core::SpriteFrame& core::Animation::getCurrentFrame() const {
    return frames.at(currentIndex);
}
