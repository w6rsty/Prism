#pragma once

#include "anim/anim.hpp"
#include "anim/anim_timer.hpp"
#include "engine/log.hpp"
#include "pecs.hpp"
#include "engine/prism.hpp"

#include "config.hpp"
#include "systems.hpp"

#include <memory>
#include <stack>
#include <string>

class RenderLayer final : public prism::Layer {
private:
    pecs::World world;
    float deltaTime_;
    float lastTime_;
    prism::anim::Translation* translatonAnim_ = nullptr;

    prism::anim::AnimationTimerManager timerManager_;

    prism::MouseMovedEvent mouseMovedEvent = prism::MouseMovedEvent(0, 0);
public:
    RenderLayer() : prism::Layer("Render Layer") {
        PRISM_INFO("[Loading RenderLayer]");
        world.AddStartupSystem(startupSystem);
        world.AddSystem(updateTickerSystem);
        world.AddSystem(updateSystem);
        world.AddSystem(renderSystem);
        PRISM_INFO("Loading resources...");

        // rotate anim         ID = 0
        timerManager_.registerTimer(1.0f);
        // translate anim      ID = 1
        timerManager_.registerTimer(2.0f);

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
        timerManager_.updateAll(deltaTime_);

        if (translatonAnim_ != nullptr) {
            prism::anim::AnimationTimer& translationTimer = timerManager_.GetTimer(1);
            if (!translationTimer.isFinished()) {
                float progress = translationTimer.getProgress();
                glm::mat4 dMat = translatonAnim_->getMat(progress);
                ModelTransform = dMat * ModelInitMat;
            } else {
                delete translatonAnim_;
                translatonAnim_ = nullptr;
            }
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
            calculateClickProjectionPos(mousePosition, ModelInitMat, camera->Position);

            translatonAnim_ = new prism::anim::Translation(2.0f, LastClickPosition, glm::vec3(0.0f, 0.0f, 0.0f), prism::anim::EaseInOut);  
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
        glm::vec4 worldPosition = glm::inverse(glm::rotate(glm::mat4(1.0f), glm::radians(30.0f), glm::vec3(1, 0, 0)) * camera->GetViewMatrix()) * viewPosition;
        glm::vec3 rayDirection = glm::normalize(glm::vec3(worldPosition) - cameraPosition);
        float t = -cameraPosition.y / rayDirection.y;
        glm::vec3 intersection = cameraPosition + t * rayDirection;
        LastClickPosition = intersection;
        // ModelTransform = glm::translate(glm::mat4(1.0f), intersection) * ModelInit;
    }
};