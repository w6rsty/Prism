#include "anim/anim_timer.hpp"

namespace prism::anim {

// 在 anim_timer.cpp 文件中
unsigned int prism::anim::TimerIDGenerator::id = 0;

AnimationTimer::AnimationTimer(float duration)
    : duration_(duration)
{
    reset();
}

void AnimationTimer::reset() {
    time_ = 0.0f;
    duration_ = 0.0f;
}

bool AnimationTimer::isFinished() {
    return time_ >= duration_;
}

void AnimationTimer::update(float deltaTime) {
    time_ += deltaTime;
}

float AnimationTimer::getProgress() {
    return time_;;
}

void AnimationTimer::modifyDuration(float duration) {
    duration_ = duration;
}

AnimationTimerManager::AnimationTimerManager() {
    timers_.reserve(10);
}

void AnimationTimerManager::registerTimer(float duration) {
    timers_.emplace(TimerIDGenerator::Get(), AnimationTimer(duration));
}

void AnimationTimerManager::updateAll(float deltaTime) {
    for (auto& [id, timer] : timers_) {
        timer.update(deltaTime);
    }
}

void AnimationTimerManager::resetAll() {
    for (auto& [id, timer] : timers_) {
        timer.reset();
    }
}

AnimationTimer& AnimationTimerManager::GetTimer(TimerID id) {
    return timers_.at(id);
}

void AnimationTimerManager::modifyTimerDuration(TimerID id, float duration) {
    timers_.at(id).modifyDuration(duration);
}
} // namespace prism::anim 