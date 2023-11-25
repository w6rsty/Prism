#pragma once

#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include "config/config.hpp"
#include <functional>
#include "engine/event/event.hpp"

namespace prism {

using EventCallbackFn = std::function<void(Event&)>;

struct WindowProps {
    const char* title;
    int width;
    int height;
    bool vsync;

    EventCallbackFn eventCallback;
};

class Window final {
public:
    Window(const WindowProps& data);
    ~Window();

    void SetSync(bool enable);

    void OnUpdate();
private:
    void initGLFW();
    void initGLAD();
    void shutdown();
private:
    GLFWwindow* window_;
    WindowProps windowData_;

    Window& operator=(const Window&) = delete;
};

} // namespace prism