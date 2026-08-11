#ifndef BOMBERMAN_CORE_ANIMATION_H
#define BOMBERMAN_CORE_ANIMATION_H

#include <vector>

namespace core {

struct SpriteFrame;

enum class AnimationType {
    Idle,
    WalkUp,
    WalkDown,
    WalkLeft,
    WalkRight,
    BombTicking,
};

class Animation {
    std::vector<SpriteFrame> frames;
    float frameDuration;
    bool loop;
    float elapsed = 0.f;
    std::size_t currentIndex = 0;
public:
    Animation(std::vector<SpriteFrame> frames, float frameDuration, bool loop = true);

    void update(float deltaTime);
    void reset();
    [[nodiscard]] const SpriteFrame& getCurrentFrame() const;

};

} // namespace core

#endif //BOMBERMAN_CORE_ANIMATION_H
