#include "anim/anim.hpp"

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
    : duration_(duration), start_(start), end_(end), curveFunc_(curveFunc) {}

glm::mat4 Translation::getMat(float t) const {
    if (curveFunc_) {
        t = curveFunc_(t, duration_);
    }
    return glm::translate(glm::mat4(1.0f), glm::mix(start_, end_, t));
}



Rotation::Rotation(float duration, glm::quat start, glm::quat end, AnimationCurveFunc curveFunc)
    : duration_(duration), start_(start), end_(end), curveFunc_(curveFunc) {}

glm::mat4 Rotation::getMat(float t) const {
    if (curveFunc_) {
        t = curveFunc_(t, duration_);
    }
    return glm::mat4_cast(glm::slerp(start_, end_, t));
}

} // namespace prism::anim

