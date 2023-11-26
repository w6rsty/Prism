#pragma once

#include "renderer.hpp"

namespace prism {

class Renderer;

class UI {
private:
    float* delta_time_;
    int windowWidth_, windowHeight_;
    bool show_demo_ = false;
    bool show_debug_ = false;
public:
    bool enable_vsync = true;
    UI(int width, int height, float* time);
    ~UI();

    void imguiLayout();
private:
    void imguiMainTabBar();
    void imguiDebugPanel(); 
    inline float getTime() const { return *delta_time_; }
};

} // namespace prism