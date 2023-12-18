#include "anim/anim_manager.hpp"
#include "anim/anim.hpp"
#include "engine/log.hpp"

namespace prism::anim {

void AnimationManager::Update(float deltaTime) {
    deltaTime_ = deltaTime;
    // play animation, if new animation is added stop and delete old one
    if (animInfos_.size() > 0) {
        auto& info  = animInfos_.front();
        if (info.animation->isDone()) {
            delete info.animation;
            animInfos_.erase(animInfos_.begin());
        } else {
            *info.target = info.animation->getMat(deltaTime_) * *info.target;
        }
    }
}

void AnimationManager::RegisterAnimation(Animation* animation, glm::mat4* target) {
    animInfos_.emplace_back(animation, target);
}

AnimationManager::~AnimationManager() {
    // if destructor is called when animation are not all done,
    // this will cause memory leak
    // animation pointer in animInfos_ are not deleted
    animInfos_.clear();
}

} // namespace prism::anim 