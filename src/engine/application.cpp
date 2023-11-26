#include "engine/application.hpp"
#include "engine/event/application_event.hpp"
#include "engine/event/event.hpp"

namespace prism {

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

Application* Application::instance = nullptr;

Application::Application() {
    instance = this;

    window_ = std::unique_ptr<Window>(Window::Create());
    window_->SetEventCallback(BIND_EVENT_FN(OnEvent));
}

Application::~Application() {

}

void Application::OnEvent(Event& event) {
    EventDispatcher dispatcher(event);
    dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));

    for (auto it = layerStack_.end(); it != layerStack_.begin();) {
        (*--it)->OnEvent(event);
        if (event.handled_) {
            break;
        }
    }
}

bool Application::OnWindowClose(WindowCloseEvent& event) {
    running_ = false;
    return true;
}

void Application::PushLayer(Layer* layer) {
    layerStack_.PushLayer(layer);
    layer->OnAttach();
}
void Application::PushOverlay(Layer* layer) {
    layerStack_.PushOverlay(layer);
    layer->OnAttach();
}

void Application::Run() {
    while (running_) {
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        for (Layer* layer : layerStack_) {
            layer->OnUpdate();
        }

        window_->OnUpdate();
    }
}


} // namespace prism