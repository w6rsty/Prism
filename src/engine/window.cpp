#include "engine/window.hpp"
#include "GLFW/glfw3.h"
#include "engine/event/application_event.hpp"
#include "engine/event/key_event.hpp"
#include "engine/event/mouse_event.hpp"
#include "engine/log.hpp"
#include <cassert>
#include <cstdio>

namespace prism {

Window::Window(const WindowProps& data) {
    PRISM_CORE_INFO("[Creating Window]");
    windowData_.title = data.title;
    windowData_.width = data.width;
    windowData_.height = data.height;
    windowData_.vsync = true;

    initGLFW();
    initGLAD();


    glfwSetWindowCloseCallback(window_, [](GLFWwindow* window) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
        WindowCloseEvent event;
        data.eventCallback(event);
    });

    glfwSetWindowSizeCallback(window_, [](GLFWwindow* window, int width, int height) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
        WindowResizeEvent event(width, height);
        PRISM_CORE_WARN(event.ToString());
        data.eventCallback(event);
    });

    glfwSetKeyCallback(window_, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

        switch (action) {
            case GLFW_PRESS: {
                KeyPressedEvent event(key, 0);
                data.eventCallback(event);
                break;
            }
            case GLFW_RELEASE: {
                KeyReleasedEvent event(key);
                data.eventCallback(event);
                break;
            }
            case GLFW_REPEAT: {
                KeyPressedEvent event(key, 1);
                data.eventCallback(event);
                break;
            }
        }
    });

    glfwSetCharCallback(window_, [](GLFWwindow* window, unsigned int keycode) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
        KeyTypedEvent event(keycode);
        data.eventCallback(event);
    });

    glfwSetMouseButtonCallback(window_, [](GLFWwindow* window, int button, int action, int mods) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
        switch (action) {
            case GLFW_PRESS: {
                MouseButtonPressedEvent event(button);
                data.eventCallback(event);
                break;
            }
            case GLFW_RELEASE: {
                MouseButtonReleasedEvent event(button);
                data.eventCallback(event);
                break;
            }
        }
    });

    glfwSetCursorPosCallback(window_, [](GLFWwindow* window, double xPos, double yPos) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
        MouseMovedEvent event(xPos, yPos);
        data.eventCallback(event);
    });

    glfwSetScrollCallback(window_, [](GLFWwindow* window, double xoffset, double yoffset) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
        MouseScrolledEvent event(xoffset, yoffset);
        data.eventCallback(event);
    });
}

Window* Window::Create(const WindowProps& data) {
    return new Window(data);
}

void Window::initGLFW() {
    if (!glfwInit()) {
        PRISM_CORE_WARN("\x1b[31;1m[GLFW ERROR]Failed to initialize GLFW\n\x1b[0m");
        assert(false);
    }

    glfwWindowHint(GLFW_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    window_ = glfwCreateWindow(windowData_.width, windowData_.height, windowData_.title, NULL, NULL);

    if (!window_) {
        PRISM_CORE_WARN("\x1b[31;1m[GLFW ERROR]Failed to initialize GLFW Window\x1b[0m\n");
        assert(false);
    }

    glfwMakeContextCurrent(window_);
    glfwSetWindowUserPointer(window_, &windowData_);
    SetSync(windowData_.vsync);
}

void Window::initGLAD() {
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        PRISM_CORE_WARN("\x1b[31;1mFailed to initialize OpenGL context\n\x1b[0m");
        assert(false);
    } else {
        std::stringstream ss;
        ss << "\x1b[32;1m[GLAD] " << glGetString(GL_VERSION) << " \x1b[0m";
        PRISM_CORE_INFO(ss.str());
    }
}

void Window::SetSync(bool enable) {
    glfwSwapInterval(enable ? 1 : 0);
    windowData_.vsync = enable;
}

void Window::OnUpdate() {
    glfwPollEvents();   
    glfwSwapBuffers(window_);
}

void Window::SetEventCallback(EventCallbackFn callback) {
    windowData_.eventCallback = callback;
}


Window::~Window() {
    shutdown();
}

void Window::shutdown() {
    glfwTerminate();
    glfwDestroyWindow(window_);
}


} // namespae prism