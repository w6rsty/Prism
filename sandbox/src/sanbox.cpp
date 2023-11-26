#include "engine/imgui/imgui_layer.hpp"
#include "engine/log.hpp"
#include "engine/application.hpp"
#include <iostream>


class RenderLayer final : public prism::Layer {
public:
    RenderLayer() : prism::Layer("Render Layer") {}

    void OnUpdate() override {
        PRISM_INFO("RenderLayer Update");
    }

    void OnEvent(prism::Event& event) override {
       PRISM_INFO("{0}", event.ToString());
    }
};

class Sandbox: public prism::Application {  
public:
    Sandbox() {
        PushLayer(new RenderLayer);
        PushOverlay(new prism::ImGuiLayer);
    }

    ~Sandbox() {

    }
};


int main(int args, char** argv) {
    prism::Log::Init();
    PRISM_CORE_INFO("\x1b[33;1m[>>>>>---Prism Engine Start---<<<<]\x1b[0m");

    auto app = new Sandbox();

    app->Run();

    delete app;
}