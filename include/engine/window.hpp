#pragma once

#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include "config/config.hpp"
#include <functional>
#include "engine/event/event.hpp"

namespace prism {

using EventCallbackFn = std::function<void(Event&)>;

struct WindowData {
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
    void SetEventCallback(EventCallbackFn callback);
    inline int GetWidth() const { return windowData_.width; }
    inline int GetHeight() const { return windowData_.height; }

    static Window* Create(const WindowProps& data = DefaultWindowProps);
private:
    void initGLFW();
    void initGLAD();
    void shutdown();
private:
    GLFWwindow* window_;
    WindowData windowData_;
};

} // namespace prism