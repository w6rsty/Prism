#include "anim/anim_manager.hpp"
#include "anim/anim.hpp"
#include "engine/log.hpp"

namespace prism::anim {

void AnimationManager::Update(float deltaTime) {
    deltaTime_ = deltaTime;
    // play animation, if new animation is added stop and delete old one
    for (auto& info: animInfos_) {
        if (info.animation->isDone()) {
            delete info.animation;
            info.animation = nullptr;
            continue;
        } else {
            *info.position = info.animation->getMat(deltaTime_) * glm::vec4(*info.position, 1.0f);
        }
    }

    // remove finished animation
    animInfos_.erase(std::remove_if(animInfos_.begin(), animInfos_.end(), [](const AnimInfo& info) {
        return info.animation == nullptr;
    }), animInfos_.end());
}

void AnimationManager::RegisterAnimation(Animation* animation, glm::vec3* postion) {
    animInfos_.emplace_back(animation, postion);
}

AnimationManager::~AnimationManager() {
    // if destructor is called when animation are not all done,
    // this will cause memory leak
    // animation pointer in animInfos_ are not deleted
    animInfos_.clear();
}

} // namespace prism::anim 