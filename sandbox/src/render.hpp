#pragma once

#include "config/config.hpp"
#include "pecs.hpp"
#include "render/camera.hpp"
#include <memory>
#include <string>
#include "engine/prism.hpp"
#include "config.hpp"

#include "player.hpp"
#include "render/shader.hpp"
#include "ticker.hpp"
#include "geo/skybox.hpp"

struct RenderInfo {
    glm::mat4 proj = glm::mat4(1.0f);
    glm::mat4 view  = glm::mat4(1.0f);
    glm::mat4* model = nullptr;

    std::shared_ptr<prism::Drawable> drawable;
    prism::ShaderType shaderType;
};

inline void updateSystem(pecs::Commands& command, pecs::Queryer queryer, pecs::Resources resources, pecs::Events& events) {
    auto entities = queryer.Query<RenderInfo>();
    auto& camera  = *resources.Get<std::shared_ptr<Camera>>();

    for (const auto& entity : entities) {
        auto& target = queryer.Get<RenderInfo>(entity);
        target.proj = glm::perspective(glm::radians(camera.Zoom), prism::ASPECT, 0.1f, 1000.0f);
        target.view = glm::rotate(glm::mat4(1.0f), glm::radians(30.0f), glm::vec3(1, 0, 0)) *  camera.GetViewMatrix();
        // target.view = camera.GetViewMatrix();
    }
}

inline void renderSystem(pecs::Commands& command, pecs::Queryer queryer, pecs::Resources resources, pecs::Events& events) {
    auto entities = queryer.Query<RenderInfo>();
    auto& camera  = *resources.Get<std::shared_ptr<Camera>>();
    auto& shaderManager = *resources.Get<std::shared_ptr<prism::ShaderManager>>();
    auto& skyboxTexture = *resources.Get<std::shared_ptr<SkyBoxTexture>>();
    auto& ticker = resources.Get<Ticker>();
    auto& shaders = shaderManager.getAllShaders();

    // pass light info
    for (const auto& shader : shaders) {
        shader->Bind();
        shader->setUniform3f("light.pos", lightPos[0], lightPos[1], lightPos[2]);
        shader->setUniform3f("light.color", lightColor[0], lightColor[1], lightColor[2]);
        shader->setUniform3f("viewPos", camera.Position.x, camera.Position.y, camera.Position.z);
    }

    for (const auto& entity : entities) {
        auto& target = queryer.Get<RenderInfo>(entity);
        
        if (target.shaderType == prism::ShaderType::HAS_TEX) {
            auto& shader = *shaderManager.getHasTexShader();
            shader.Bind();
            shader.setUniformMat4f("proj_matrix", target.proj);
            shader.setUniformMat4f("view_matrix", target.view);
            glm::mat4 modelMat = *target.model;
            target.drawable->onRender(shader);
            shader.setUniformMat4f("model_matrix", modelMat);
        } 
        if (target.shaderType == prism::ShaderType::WITH_TEX) {
            auto& shader = *shaderManager.getWithTexShader();
            auto& tex = resources.Get<std::shared_ptr<Texture>>();
            shader.Bind();
            tex->Bind(0);
            skyboxTexture.Bind(1);
            shader.setUniform1i("tex", 0);
            shader.setUniform1i("skybox", 1);
            shader.setUniformMat4f("proj_matrix", target.proj);
            shader.setUniformMat4f("view_matrix", target.view);
            glm::mat4 modelMat = *target.model;
            shader.setUniformMat4f("model_matrix", modelMat);
            target.drawable->onRender(shader);
            skyboxTexture.Unbind();
        }
    }

    auto& skybox = *resources.Get<std::shared_ptr<prism::SkyBox>>();
    auto& shader = *shaderManager.getSkyboxShader();
    shader.Bind();
    skyboxTexture.Bind(0);
    shader.setUniform1i("skybox", 0);
    auto skyboxView = glm::mat4(glm::mat3(glm::rotate(glm::mat4(1.0f), glm::radians(30.0f), glm::vec3(1, 0, 0)) * camera.GetViewMatrix()));
    shader.setUniformMat4f("view_matrix", skyboxView);
    auto projection = glm::perspective(glm::radians(camera.Zoom), prism::ASPECT, 0.1f, 1000.0f);
    shader.setUniformMat4f("proj_matrix", projection);
    skybox.onRender(shader);
    skyboxTexture.Unbind();
}