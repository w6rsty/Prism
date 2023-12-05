#pragma once

#include <vector>
#include <cstdint>
#include <unordered_map>

namespace prism::anim {

using TimerID = uint32_t;

class TimerIDGenerator {
private:
    static TimerID id;
public:
    inline static TimerID Get() {
        return id++;
    }
};

class AnimationTimer {
private:
    uint32_t id_ = TimerIDGenerator::Get();
    float time_;
    float duration_;

public:
    AnimationTimer(float duration);
    void update(float deltaTime);
    float getProgress();
    bool isFinished();
    void reset();
    void modifyDuration(float duration);
    inline TimerID getID() const { return id_; }
};

class AnimationTimerManager {
private:
    std::unordered_map<TimerID, AnimationTimer> timers_;
public:
    AnimationTimerManager();
    void registerTimer(float duration);
    void updateAll(float deltaTime);
    void resetAll();
    AnimationTimer& GetTimer(TimerID id);
    void modifyTimerDuration(TimerID id, float duration);
};

} // namespace prism::anim