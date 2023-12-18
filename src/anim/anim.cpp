#include "anim/anim.hpp"
#include "engine/log.hpp"

namespace prism::anim {

float Linear(float t, float duration) {
    return t / duration;
}

float EaseIn(float t, float duration) {
    return t * t * t / (duration * duration * duration);
}

float EaseOut(float t, float duration) {
    float t1 = t / duration - 1;
    return t1 * t1 * t1 + 1;
}

float EaseInOut(float t, float duration) {
    float t1 = t / (duration / 2);
    if (t1 < 1) {
        return 0.5f * t1 * t1 * t1;
    }
    t1 -= 2;
    return 0.5f * (t1 * t1 * t1 + 2);
}



Translation::Translation(float duration, glm::vec3 start, glm::vec3 end, AnimationCurveFunc curveFunc)
    : duration_(duration), start_(start), end_(end), curveFunc_(curveFunc) 
{
    length_ = glm::length(end_ - start_);
    glm::vec3 dir = end_ - start_;
    if (glm::length(dir) < 0.0001f) {
        direction_ = glm::vec3(0.0f, 0.0f, 0.0f);
    } else {
        direction_ = glm::normalize(dir);
    }
}

glm::mat4 Translation::getMat(float t) {
    playingTime_ += t;
    float progress = curveFunc_(playingTime_, duration_);
    float dp = progress - lastProgressTime;
    lastProgressTime = progress;
    // return a delta translation matrix
    return glm::translate(glm::mat4(1.0f), direction_ * length_ * dp);
}

void Translation::setSpeed(float speed) {
    duration_ = length_ / speed;
}


Rotation::Rotation(float duration, glm::quat start, glm::quat end, AnimationCurveFunc curveFunc)
    : duration_(duration), start_(start), end_(end), curveFunc_(curveFunc) {}

glm::mat4 Rotation::getMat(float t) {
    playingTime_ += t;
    float progress = curveFunc_(playingTime_, duration_);
    glm::quat thisRotation = glm::slerp(start_, end_, progress);
    glm::quat lastRotation = glm::slerp(start_, end_, lastProgressTime);
    lastProgressTime = progress;

    glm::quat deltaRotation = thisRotation * glm::inverse(lastRotation);

    return glm::mat4_cast(deltaRotation);
}

} // namespace prism::anim

