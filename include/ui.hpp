#pragma once

#include "renderer.hpp"

namespace prism {

class Renderer;

class UI {
private:
    Renderer* _rd;
public:
    UI(Renderer* rd);

    void imguiInit();
    void imguiLayout();
    void imguiMainTabBar();
    void imguiDebugPanel();
};

} // namespace prism