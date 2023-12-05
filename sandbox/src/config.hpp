#pragma once

#include "config/config.hpp"
#include "engine/prism.hpp"
#include "render/camera.hpp"
#include "render/vertex_array.hpp"

static Camera* camera = new Camera({0, 15, 15});
static float xpos = (float)prism::WIDTH / 2;
static float ypos = (float)prism::HEIGHT / 2; 
static bool firstMosue = true;
static GLFWwindow* window = nullptr;
static std::pair<float, float> mousePosition = std::make_pair(0, 0);

// Model path
static const char* modelNanosuitPath = "D:/home/Prism/resources/model/nanosuit/nanosuit.obj";
static const char* modelBackpackPath = "D:/home/Prism/resources/model/backpack/backpack.obj";

static glm::vec3 ModelInitPostion = glm::vec3(0.0f, 0.0f, 0.0f);
static glm::mat4 ModelInitMat = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f));
static glm::mat4 ModelTransform = glm::mat4(1.0f) * ModelInitMat;

static glm::vec3 LastClickPosition = ModelInitPostion;

static float lightPos[3] { 2, 5, 15 };
static float lightColor[3] { 1, 1, 1 };

// skybox faces texture path
static std::vector<std::string> faces {
    "D:/home/Prism/resources/skybox/right.jpg",
    "D:/home/Prism/resources/skybox/left.jpg",
    "D:/home/Prism/resources/skybox/top.jpg",
    "D:/home/Prism/resources/skybox/bottom.jpg",
    "D:/home/Prism/resources/skybox/front.jpg",
    "D:/home/Prism/resources/skybox/back.jpg",
};
static glm::mat4 skyboxTransfrom = glm::mat4(1.0f);

static glm::mat4 groundModelTransform = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f));

// Resigter shader compile info
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

// camera movement
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
}