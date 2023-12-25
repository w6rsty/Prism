#pragma once

#include <vector>

#include "anim/anim.hpp"

namespace prism::anim {

class AnimationManager {
private:
    float deltaTime_;

    struct AnimInfo {
        Animation* animation;
        glm::vec3* position;
    };

    std::vector<AnimInfo> animInfos_;
public:
    void Update(float deltaTime);
    void RegisterAnimation(Animation* animation, glm::vec3 *position);
    inline bool isAnimating() const { return animInfos_.size() > 1; }

    ~AnimationManager();
};

} // namespace prism::anim