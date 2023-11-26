#pragma once

#include "engine/event/application_event.hpp"
#include "engine/event/event.hpp"
#include "engine/event/key_event.hpp"
#include "engine/event/mouse_event.hpp"
#include "engine/layer.hpp"
#include "engine/ui.hpp"
#include <memory>


namespace prism {

class UI;

class ImGuiLayer final : public Layer {
private:
    float delta_time_ = 0.0f;
    float time_ = 0.0f; 
    std::unique_ptr<UI> ui_;
public:
    ImGuiLayer();
    ~ImGuiLayer();

    void OnAttach() override;
    void OnDetach() override;
    void OnUpdate() override;
    void OnEvent(Event& event) override;
private:
    bool OnMouseButtonPressedEvent(MouseButtonEvent& event);
    bool OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& event);
    bool OnMouseMovedEvent(MouseMovedEvent& event);
    bool OnMouseScrolledEvent(MouseScrolledEvent& event);
    bool OnKeyPresssedEvent(KeyPressedEvent& event);
    bool OnKeyReleasedEvent(KeyReleasedEvent& event);
    bool OnKeyTypedEvent(KeyTypedEvent& event);
    bool OnWindowResizeEvent(WindowResizeEvent& event);
};

} // namespace prism
