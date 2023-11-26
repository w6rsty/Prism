#include "pecs.hpp"
#include "engine/prism.hpp"
#include "render/camera.hpp"
#include "render/drawable.hpp"
#include "render/model.hpp"
#include "render/shader.hpp"

#include <iostream>
#include <memory>
#include <string>

struct Player {
    std::string name;
};

struct HP {
    float value;
    float maxValue;
};

struct Position {
    float x, y, z;
};

void startupSystem(pecs::Commands& command) {
    command.Spawn(Player{"w6rsty"}, HP {100, 100}, Position {0.0f, 0.0f, 0.0f})
           .Spawn(Player{"genshin"}, HP {100, 100}, Position {1.0f, 2.0f, 3.0f})
           ;
}

void updateSystem(pecs::Commands& command, pecs::Queryer queryer, pecs::Resources resources, pecs::Events& events) {
    auto entities = queryer.Query<Player>();
    for (const auto& entity : entities) {
        auto& entity_name = queryer.Get<Player>(entity).name; 
        auto& entity_position = queryer.Get<Position>(entity);
    }
}

class RenderLayer final : public prism::Layer {
private:
    pecs::World world;
    std::shared_ptr<Shader> shader_;
    std::shared_ptr<prism::Drawable> draw_;
    std::shared_ptr<Camera> camera_;
    glm::mat4 projMat_;
    glm::mat4 viewMat_;
    glm::mat4 modelMat_;

public:
    RenderLayer() : prism::Layer("Render Layer") {
        world.AddStartupSystem(startupSystem)
             .AddSystem(updateSystem);
        world.Startup();
        
        shader_ = std::make_shared<Shader>(prism::meshVertexPath, prism::meshFragPath);
        draw_ = std::make_shared<prism::Model>("D:/home/Prism/resources/model/backpack/backpack.obj", false);
        auto camera = new Camera({0, 1, 5});
        camera_ = std::shared_ptr<Camera>(camera);
    }


    ~RenderLayer() {
        world.Shutdown();
    }

    void OnUpdate() override {
        shader_->Bind();
        projMat_ = glm::perspective(glm::radians(camera_->Zoom), 1920.0f/ 1080.0f, 0.1f, 1000.0f);
        viewMat_ = camera_->GetViewMatrix();
        modelMat_ = glm::rotate(glm::mat4(1.0f), (float)glfwGetTime(), glm::vec3(0, 1, 0)   );

        shader_->setUniformMat4f("proj_matrix", projMat_);
        shader_->setUniformMat4f("view_matrix", viewMat_);
        shader_->setUniformMat4f("model_matrix", modelMat_);
        shader_->setUniform3f("light.pos", 2, 2, 2);
        shader_->setUniform3f("light.color", 1, 1, 1);
        draw_->onRender(*shader_);
        world.Update();
    }
    

    void OnEvent(prism::Event& event) override {
       PRISM_INFO("{}", event.ToString());
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

    app->Run();

    PRISM_WARN("Now deleting app...");
    delete app;
    PRISM_WARN("Reach the end of program");
}