#pragma once

#include "config/config.hpp"
#include "engine/prism.hpp"
#include "render/camera.hpp"
#include "render/vertex_array.hpp"

// Model path
static const char* modelNanosuitPath = "D:/home/Prism/resources/model/nanosuit/nanosuit.obj";
static const char* modelBackpackPath = "D:/home/Prism/resources/model/backpack/backpack.obj";

static Camera* camera = new Camera({0, 5, 10});
static float xpos = (float)prism::WIDTH / 2;
static float ypos = (float)prism::HEIGHT / 2; 
static bool firstMosue = true;
static GLFWwindow* window = nullptr;

static glm::mat4 movement(1.0f);

static std::vector<std::string> faces {
    "D:/home/Prism/resources/skybox/right.jpg",
    "D:/home/Prism/resources/skybox/left.jpg",
    "D:/home/Prism/resources/skybox/top.jpg",
    "D:/home/Prism/resources/skybox/bottom.jpg",
    "D:/home/Prism/resources/skybox/front.jpg",
    "D:/home/Prism/resources/skybox/back.jpg",
};

// Config shader and compile options
static std::vector<prism::CreateShaderInfo> createShaderInfo = {
    prism::CreateShaderInfo{
        .vertexPath = prism::skyboxVertexPath,
        .fragPath = prism::skyboxFragPath,
        .type = prism::ShaderType::SKYBOX,
    },
    prism::CreateShaderInfo{
        .vertexPath = prism::meshVertexPath,
        .fragPath = prism::meshFragPath,
        .type = prism::ShaderType::HAS_TEX,
    },
    prism::CreateShaderInfo{
        .vertexPath = prism::withTexVertexPath,
        .fragPath = prism::withTexFragPath,
        .type = prism::ShaderType::WITH_TEX,
    },
};

enum class ModelMove {
    FORWARD, BACKWARD, LEFT, RIGHT
};

inline void ProcessModelMove(glm::mat4& mat, ModelMove move, float deltaTime) {
    glm::vec3 direction(0.0f);
    if (move == ModelMove::FORWARD) {
        direction = glm::vec3(0, 0, 1);
    }
    if (move == ModelMove::BACKWARD) {
        direction = glm::vec3(0, 0, -1);
    }
    if (move == ModelMove::LEFT) {
        direction = glm::vec3(1, 0, 0);
    }
    if (move == ModelMove::RIGHT) {
        direction = glm::vec3(-1, 0, 0);
    }
    mat = glm::translate(mat, direction * 10.0f * deltaTime);
}

inline void handleGenaralKeyboardInput(float deltaTime) {
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera->ProcessKeyboard(FORWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera->ProcessKeyboard(BACKWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera->ProcessKeyboard(LEFT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera->ProcessKeyboard(RIGHT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        camera->ProcessKeyboard(UP, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
        camera->ProcessKeyboard(DOWN, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        ProcessModelMove(movement, ModelMove::FORWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        ProcessModelMove(movement, ModelMove::BACKWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        ProcessModelMove(movement, ModelMove::LEFT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        ProcessModelMove(movement, ModelMove::RIGHT, deltaTime);
    }
}

