#include "engine/window.hpp"
#include "GLFW/glfw3.h"
#include "engine/event/key_event.hpp"
#include "engine/event/mouse_event.hpp"
#include <cassert>
#include <cstdio>

namespace prism {

Window::Window(const WindowProps& data) {
    windowData_.title = data.title;
    windowData_.width = data.width;
    windowData_.height = data.height;
    windowData_.vsync = data.vsync;
    windowData_.eventCallback = data.eventCallback;

    initGLFW();
    initGLAD();


    glfwSetKeyCallback(window_, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
        WindowProps& data = *(WindowProps*)glfwGetWindowUserPointer(window);

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
        WindowProps& data = *(WindowProps*)glfwGetWindowUserPointer(window);
        KeyTypedEvent event(keycode);
        data.eventCallback(event);
    });

    glfwSetCursorPosCallback(window_, [](GLFWwindow* window, double xPos, double yPos) {
        WindowProps& data = *(WindowProps*)glfwGetWindowUserPointer(window);
        MouseMovedEvent event(xPos, yPos);
    });

    glfwSetScrollCallback(window_, [](GLFWwindow* window, double xoffset, double yoffset) {
        WindowProps& data = *(WindowProps*)glfwGetWindowUserPointer(window);
        MouseScrolledEvent event(xoffset, yoffset);
        data.eventCallback(event);
    });
}

void Window::initGLFW() {
    if (!glfwInit()) {
        printf("\x1b[31;1m[GLFW ERROR]Failed to initialize GLFW\n\x1b[0m\n");
        assert(false);
    }

    glfwWindowHint(GLFW_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    window_ = glfwCreateWindow(windowData_.width, windowData_.height, windowData_.title, NULL, NULL);

    if (!window_) {
        printf("\x1b[31;1m[GLFW ERROR]Failed to initialize GLFW Window\n\x1b[0m\n");
        assert(false);
    }

    glfwMakeContextCurrent(window_);
    glfwSetWindowUserPointer(window_, &windowData_);
    SetSync(windowData_.vsync);
}

void Window::initGLAD() {
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        printf("\x1b[31;1mFailed to initialize OpenGL context\n\x1b[0m\n");
        assert(false);
    } else {
        printf("[GLAD] %s", glGetString(GL_VERSION));
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

Window::~Window() {
    shutdown();
}

void Window::shutdown() {
    glfwDestroyWindow(window_);
}


} // namespae prism