#include "geo/skybox.hpp"
#include "pecs.hpp"
#include "engine/prism.hpp"

#include "config.hpp"
#include "render/drawable.hpp"
#include "render/texture.hpp"
#include "systems.hpp"

#include <memory>
#include <string>

static glm::mat4 backpackModelTransform = glm::translate(glm::mat4(1.0f), glm::vec3(2, 2, 0));
static glm::mat4 groundModelTransform = glm::scale(glm::mat4(1.0f), glm::vec3(40.0f));
static glm::mat4 skyboxTransfrom = glm::mat4(1.0f);

void startupSystem(pecs::Commands& command) {
    command.Spawn<Player, RenderInfo>(
        Player{"w6rsty"},
        RenderInfo{
            .model = &movement,
            .drawable = std::make_shared<prism::Model>(modelNanosuitPath, true),
            .shaderType = prism::ShaderType::HAS_TEX,
        }
    );
    command.Spawn<RenderInfo>(
        RenderInfo{
            .model = &groundModelTransform,
            .drawable = std::make_shared<prism::Plane>(2.0f),
            .shaderType = prism::ShaderType::WITH_TEX
        }
    );
    command.Spawn<RenderInfo>(
        RenderInfo{
            .model = &skyboxTransfrom,
            .drawable = std::make_shared<prism::SkyBox>(faces),
            .shaderType = prism::ShaderType::SKYBOX
        }
    );

    command.SetResource(std::shared_ptr<Camera>(camera));
    command.SetResource(std::make_shared<prism::ShaderManager>(createShaderInfo));
    command.SetResource(Ticker{});


    int bufw = 512, bufh = 512;
    unsigned char* buffer = new unsigned char[bufw * bufh * 3];
    createCheckboardTexture(buffer, bufw, bufh, 128); 
    command.SetResource(std::make_shared<Texture>(bufw, bufh, buffer));
    delete[] buffer;
}

class RenderLayer final : public prism::Layer {
private:
    pecs::World world;
    float deltaTime_;
    float lastTime_;
public:
    RenderLayer() : prism::Layer("Render Layer") {
        PRISM_INFO("[Loading RenderLayer]");
        world.AddStartupSystem(startupSystem);
        world.AddSystem(updateTickerSystem);
        world.AddSystem(updateSystem);
        world.AddSystem(renderSystem);
        world.Startup();
    }

    ~RenderLayer() {
        world.Shutdown();
    }

    void OnUpdate() override {
        float currentTime = glfwGetTime();
        deltaTime_ = currentTime - lastTime_;
        lastTime_ = currentTime;

        handleGenaralKeyboardInput(deltaTime_);

        world.Update();
    }

    void OnEvent(prism::Event& event) override {
        // if (event.IsInCategory(prism::EventCategoryMouseButton)) {
        //     prism::MouseButtonPressedEvent& mouseButton = static_cast<prism::MouseButtonPressedEvent&>(event);
        //     if (mouseButton.GetMouseButton() == GLFW_MOUSE_BUTTON_1) {

        //     }
        // }
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
    PRISM_CORE_INFO("\x1b[35;1m[>>>>>---Prism Log System Start---<<<<]\x1b[0m");

    auto app = new Sandbox();
    window = app->GetWindow().GetWindowPtr();

    app->Run();
}