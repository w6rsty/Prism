#pragma once

#include <vector>

#include "anim/anim.hpp"

namespace prism::anim {

class AnimationManager {
private:
    float deltaTime_;

    struct AnimInfo {
        Animation* animation;
        glm::mat4* target;
    };

    std::vector<AnimInfo> animInfos_;
public:
    void Update(float deltaTime);

    void RegisterAnimation(Animation* animation, glm::mat4* target);

    ~AnimationManager();
};

} // namespace prism::anim