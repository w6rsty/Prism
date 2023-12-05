#pragma once

#include "glm/fwd.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"

namespace prism::anim {

enum class AnimationCurveType {
    LINEAR,
    EASE_IN,
    EASE_OUT,
    EASE_IN_OUT,
};

using AnimationCurveFunc = float(*)(float, float);

float Linear(float t, float duration);
float EaseIn(float t, float duration);
float EaseOut(float t, float duration);
float EaseInOut(float t, float duration);

class Animation {
public:
    virtual ~Animation() = default;
    virtual glm::mat4 getMat(float t) const = 0;
};

class Translation final : public Animation {
private:
    float duration_;
    glm::vec3 start_;
    glm::vec3 end_;

    AnimationCurveFunc curveFunc_;
public:
    Translation(float duration, glm::vec3 start, glm::vec3 end, AnimationCurveFunc curveFunc = nullptr);

    glm::mat4 getMat(float t) const override;
};

class Rotation final : public Animation {
private:
    float duration_;
    glm::quat start_;
    glm::quat end_;

    AnimationCurveFunc curveFunc_;
public:
    Rotation(float duration, glm::quat start, glm::quat end, AnimationCurveFunc curveFunc = nullptr);
    glm::mat4 getMat(float t) const override;
};
} // namespace prism::anim
