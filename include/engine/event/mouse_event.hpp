#pragma once

#include "engine/event/event.hpp"
#include "engine/event/mouse_event.hpp"
#include <sstream>

namespace prism {

class MouseMovedEvent final : public Event {
private:
    float mouseX_, mouseY_;
public:
    MouseMovedEvent(float x, float y)
        : mouseX_(x), mouseY_(y) {}
    inline float GetX() const { return mouseX_; }
    inline float GetY() const { return mouseY_; }

    std::string ToString() const override {
        std::stringstream ss;
        ss << "MouseMovedEvent: " << mouseX_ << ", " << mouseY_;
        return ss.str();
    }

    EVENT_CLASS_TYPE(MouseMoved)
    EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
};

class MouseScrolledEvent final : public Event {
private:
    float xOffset_, yOffset;
public:
    MouseScrolledEvent(float x, float y)
        : xOffset_(x), yOffset(y) {}
    inline float GetXOffset() const { return xOffset_; }
    inline float GetYOffset() const { return yOffset; }

    std::string ToString() const override {
        std::stringstream ss;
        ss << "MouseScrolledEvent: " << xOffset_ << ", " << yOffset;
        return ss.str();
    }

    EVENT_CLASS_TYPE(MouseScrolled)
    EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)  
};

class MouseButtonEvent : public Event {
protected:
    MouseButtonEvent(int button) : button_(button) {}

    int button_;
public:
    inline int GetMouseButton() const { return button_; }
    EVENT_CLASS_CATEGORY(EventCategoryMouseButton | EventCategoryInput)
};

class MouseButtonPressedEvent final : public MouseButtonEvent {
public:
    MouseButtonPressedEvent(int button) : MouseButtonEvent(button) {}

    std::string ToString() const override {
        std::stringstream ss;
        ss << "MouseButtonPressedEvent: " << button_;
        return ss.str();
    }
    
    EVENT_CLASS_TYPE(MouseButtonPressed)
};

class MouseButtonReleasedEvent final : public MouseButtonEvent {
public:
    MouseButtonReleasedEvent(int button) : MouseButtonEvent(button) {}

    std::string ToString() const override {
        std::stringstream ss;
        ss << "MouseButtonReleasedEvent: " << button_;
        return ss.str();
    }

    EVENT_CLASS_TYPE(MouseButtonReleased)
};

} // namespace prism