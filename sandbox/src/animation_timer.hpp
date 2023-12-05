#pragma once

#include "pecs.hpp"
#include "ticker.hpp"

struct AnimTimer {
    int tick;
    int duration_tick;
};

inline void updateAnimTimerSystem(pecs::Commands& command, pecs::Queryer queryer, pecs::Resources resources, pecs::Events& events) {
    auto entities = queryer.Query<AnimTimer>(); 
    auto& ticker = resources.Get<Ticker>();
    // increase time
    for (const auto& entity : entities) {
        auto& animTimer = queryer.Get<AnimTimer>(entity);
        animTimer.tick += ticker.speed;

        if (animTimer.tick >= animTimer.duration_tick) {
            command.Destory(entity);
        }
    }
}