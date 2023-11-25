#pragma once

#include "engine/event/event.hpp"
#include <sstream>

namespace prism {

class KeyEvent : public Event {
protected:
    int keyCode_;

    KeyEvent(int keycode) : keyCode_(keycode) {}
public:
    inline int GetKeyCode() const { return keyCode_; }

    EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)
};

class KeyPressedEvent final : public KeyEvent {
private:
    int repeatCount_;
public:
    KeyPressedEvent(int keycode, int repeatCount)
        : KeyEvent(keycode), repeatCount_(repeatCount) {}

    inline int GetRepeatCount() const { return repeatCount_; }

    std::string ToString() const override {
        std::stringstream ss;
        ss << "KeyPressedEvent: " << keyCode_ << "(" << repeatCount_ <<" repeats)"; 
        return ss.str();
    }

    EVENT_CLASS_TYPE(KeyPressed)
};

class KeyReleasedEvent final : public KeyEvent {
public:
    KeyReleasedEvent(int keycode)
        : KeyEvent(keycode) {}

    std::string ToString() const override {
        std::stringstream ss;
        ss << "KeyReleasedEvent: " << keyCode_; 
        return ss.str();
    }

    EVENT_CLASS_TYPE(KeyReleased)
};

class KeyTypedEvent final : public KeyEvent {
public:
    KeyTypedEvent(int keycode)
        : KeyEvent(keycode) {}

    std::string ToString() const override {
        std::stringstream ss;
        ss << "KeyReleasedEvent: " << keyCode_; 
        return ss.str();
    }

    EVENT_CLASS_TYPE(KeyTyped)
};

} // namespace prism