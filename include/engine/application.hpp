#pragma once

#include "engine/event/application_event.hpp"
#include "engine/layer.hpp"
#include "engine/layer_stack.hpp"
#include "engine/window.hpp"
#include <memory>

#include "engine/event/event.hpp"

namespace prism {

class Application {
private:
    static Application*     instance;
    bool                    running_ = { true };
    LayerStack              layerStack_;

    std::unique_ptr<Window> window_;
public:
    Application();
    ~Application();

    void Run();
    void OnEvent(Event& event);
    void PushLayer(Layer* layer);
    void PushOverlay(Layer* layer);

    inline static Application* GetInstance() { return instance; }
    inline Window& GetWindow() { return *window_; }
private:
    bool OnWindowClose(WindowCloseEvent& event);
};

Application* CreateApplication();

} // namespace prism