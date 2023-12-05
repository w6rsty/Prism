#include "render_layer.hpp"

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
    PRISM_CORE_INFO("\x1b[35;1m[>>>>>---Prism Log System Start---<<<<]\x1b[0m");

    auto app = new Sandbox();
    window = app->GetWindow().GetWindowPtr();

    app->Run();
}