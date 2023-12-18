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

enum class AnimationType {
    Translation,
    Rotation,
};

using AnimationCurveFunc = float(*)(float, float);

float Linear(float t, float duration);
float EaseIn(float t, float duration);
float EaseOut(float t, float duration);
float EaseInOut(float t, float duration);

class Animation {
public:
    virtual ~Animation() = default;
    virtual glm::mat4 getMat(float t) = 0;
    virtual float getDuration() const = 0;
    virtual bool isDone() const = 0;
    virtual AnimationType getType() const = 0;
    virtual void setSpeed(float speed) {}
};

class Translation final : public Animation {
private:
    float duration_;
    glm::vec3 start_;
    glm::vec3 end_;
    float lastProgressTime = 0.0f;
    float playingTime_ = 0.0f;

    AnimationCurveFunc curveFunc_;

    float length_;
    glm::vec3 direction_;
public:
    Translation(float duration, glm::vec3 start, glm::vec3 end, AnimationCurveFunc curveFunc = nullptr);

    glm::mat4 getMat(float t) override;
    inline float getDuration() const override { return duration_; }
    inline bool isDone() const override { return playingTime_ > duration_; }
    inline AnimationType getType() const override { return AnimationType::Translation; }
    void setSpeed(float speed) override; 
};

class Rotation final : public Animation {
private:
    float duration_;
    glm::quat start_;
    glm::quat end_;
    float lastProgressTime = 0.0f;
    float playingTime_ = 0.0f;
    AnimationCurveFunc curveFunc_;

    float length_;
    glm::vec3 direction_;
public:
    Rotation(float duration, glm::quat start, glm::quat end, AnimationCurveFunc curveFunc = nullptr);
    glm::mat4 getMat(float t) override;
    inline float getDuration() const override { return duration_; }
    inline bool isDone() const override { return playingTime_ > duration_; }
    inline AnimationType getType() const override { return AnimationType::Rotation; } 
};
} // namespace prism::anim
