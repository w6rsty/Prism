#pragma once

#include "pecs.hpp"
#include <iostream>

using namespace pecs;

struct Ticker {
    unsigned long long tick = 0;
    unsigned long long speed = 1;
};

inline void startupTickerSystem(Commands& command) {
    command.SetResource(Ticker{ .tick = 0, .speed = 1 });
}

inline void updateTickerSystem(Commands& command, Queryer queryer, Resources resources, Events& events) {
    auto& ticker = resources.Get<Ticker>();
    ticker.tick += ticker.speed;
}

inline void echoTickerSystem(Commands& command, Queryer queryer, Resources resources, Events& events) {
    auto& ticker = resources.Get<Ticker>();
    std::cout << "Tikcer: " << ticker.tick << std::endl;
}

struct Timer {
    int tick = 0;
    int max = 100;
};

inline void startupTimerSystem(Commands& command) {
    command.SetResource(Timer{ .tick = 0, .max = 500 });
}