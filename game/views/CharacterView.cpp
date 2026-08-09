#include "CharacterView.h"

#include "Stopwatch.h"

void CharacterView::update() {
    EntityView::update();
    if (!currentAnimation.empty()) {
        if (const auto it = animations.find(currentAnimation); it != animations.end()) {
            it->second.update(core::Stopwatch::getInstance().getDeltaTime());
            sprite = it->second.getCurrentFrame();
        }
    }
}

void CharacterView::setAnimation(const std::string& animationKey) {
    currentAnimation = animationKey;
}
