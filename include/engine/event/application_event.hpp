#pragma once

#include "engine/event/event.hpp"
#include <sstream>

namespace prism {

class WindowResizeEvent final : public Event {
private:
    unsigned int width_, height_;
public:
    WindowResizeEvent(unsigned int width, unsigned int height)
        : width_(width), height_(height) {}
    inline unsigned int GetWidth() const { return width_; }
    inline unsigned int GetHeight() const { return height_; }

    std::string ToString() const override {
        std::stringstream ss;
        ss << "WindowResizeEvent: " << width_ << ", " << height_;
        return ss.str();
    }

    EVENT_CLASS_TYPE(WindowResized)
    EVENT_CLASS_CATEGORY(EventCategoryApplication)
};

class WindowCloseEvent final : public Event {
public:
    WindowCloseEvent() {}

    EVENT_CLASS_TYPE(WindowClosed)
    EVENT_CLASS_CATEGORY(EventCategoryApplication)
};

class AppTickEvent final : public Event {
public:
    AppTickEvent() {}

    EVENT_CLASS_TYPE(AppTick)
    EVENT_CLASS_CATEGORY(EventCategoryApplication)
};

class AppUpdateEvent final : public Event {
public:
    AppUpdateEvent() {}

    EVENT_CLASS_TYPE(AppUpdate)
    EVENT_CLASS_CATEGORY(EventCategoryApplication)
};

class AppRenderEvent final : public Event {
public:
    AppRenderEvent() {}

    EVENT_CLASS_TYPE(AppRender)
    EVENT_CLASS_CATEGORY(EventCategoryApplication)
};

} // namespace prism