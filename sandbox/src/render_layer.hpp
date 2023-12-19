#pragma once

#include "anim/anim.hpp"
#include "anim/anim_manager.hpp"
#include "engine/log.hpp"
#include "pecs.hpp"
#include "engine/prism.hpp"

#include "config.hpp"
#include "systems.hpp"

#include <memory>
#include <string>

static int randInt(int max, int min = 0) {
    static std::mt19937 engine(std::random_device{}());
    std::uniform_int_distribution<int> distribution(min, max);
    return distribution(engine);
}

class RenderLayer final : public prism::Layer {
private:
    pecs::World world;
    float deltaTime_;
    float lastTime_;
    prism::anim::Translation* translatonAnim_ = nullptr;
    prism::anim::AnimationManager animManager_;

    prism::MouseMovedEvent mouseMovedEvent = prism::MouseMovedEvent(0, 0);
public:
    RenderLayer() : prism::Layer("Render Layer") {
        PRISM_INFO("[Loading RenderLayer]");
        world.AddStartupSystem(startupSystem);
        world.AddSystem(updateTickerSystem);
        world.AddSystem(updateSystem);
        world.AddSystem(renderSystem);

        PRISM_INFO("[PECS] World Startup!");
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
        animManager_.Update(deltaTime_);

        if (!animManager_.isAnimating()) {
            randomWalk();
        }

        world.Update();
    }

    void OnEvent(prism::Event& event) override {
        prism::EventDispatcher dispatcher(event);
        dispatcher.Dispatch<prism::MouseButtonPressedEvent>(PRISM_BIND_EVENT_FN(RenderLayer::OnMouseButtonPressed));
        dispatcher.Dispatch<prism::MouseMovedEvent>(PRISM_BIND_EVENT_FN(RenderLayer::OnMouseMoved));
    }

    bool OnMouseButtonPressed(prism::MouseButtonPressedEvent& event) {
        if (event.GetMouseButton() == GLFW_MOUSE_BUTTON_1) {
            mousePosition = {mouseMovedEvent.GetX(), mouseMovedEvent.GetY()};
            calculateClickProjectionPos(mousePosition, glm::mat4(1.0f), camera->Position);
        }
        return false;
    }

    bool OnMouseMoved(prism::MouseMovedEvent& event) {
        // Update the current mouse position
        mouseMovedEvent = event;
        return false;
    }
    void calculateClickProjectionPos(const std::pair<float, float>& mousePosition, const glm::mat4& ModelInit, const glm::vec3& cameraPosition) {
        glm::vec4 clipSpacePosition = glm::vec4(2.0f * mousePosition.first / prism::WIDTH - 1.0f, 1.0f - 2.0f * mousePosition.second / prism::HEIGHT, -1.0f, 1.0f);
        glm::vec4 viewPosition = glm::inverse(glm::perspective(glm::radians(camera->Zoom), prism::ASPECT, 0.1f, 1000.0f)) * clipSpacePosition;
        viewPosition = viewPosition / viewPosition.w;
        glm::vec4 worldPosition = glm::inverse(glm::rotate(glm::mat4(1.0f), glm::radians(CameraRotation), glm::vec3(1, 0, 0)) * camera->GetViewMatrix()) * viewPosition;
        glm::vec3 rayDirection = glm::normalize(glm::vec3(worldPosition) - cameraPosition);
        float t = -cameraPosition.y / rayDirection.y;
        glm::vec3 intersection = cameraPosition + t * rayDirection;
        intersection.y = ModelPosition.y;

        if (abs(intersection.x) < 20.0f && abs(intersection.z) < 20.0f) {
            if (ModelPosition != intersection) {
                auto trans = new prism::anim::Translation(0.5f, ModelPosition, intersection, prism::anim::Linear);
                trans->setSpeed(ModelSpeed);
                animManager_.RegisterAnimation(trans, &ModelTransform);
                ModelPosition = intersection;
            } 
        }
    }
    void randomWalk() {
        int randx = randInt(20, -20);
        int randz = randInt(20, -20);
        glm::vec3 randpos(randx, ModelPosition.y, randz);
        // register a translation animation
        if (ModelPosition != randpos) {
                auto trans = new prism::anim::Translation(0.5f, ModelPosition, randpos, prism::anim::Linear);
                trans->setSpeed(ModelSpeed);
                animManager_.RegisterAnimation(trans, &ModelTransform);
                ModelPosition = randpos;
        } 
    }
};