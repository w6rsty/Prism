#pragma once

#include "config/config.hpp"
#include "engine/prism.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/fwd.hpp"
#include "render/camera.hpp"
#include "render/vertex_array.hpp"
#include <memory>

static Camera* camera = new Camera({0, 32, 32});
static float CameraRotation = 45.0f;
static float xpos = (float)prism::WIDTH / 2;
static float ypos = (float)prism::HEIGHT / 2; 
static bool firstMosue = true;
static GLFWwindow* window = nullptr;
static std::pair<float, float> mousePosition = std::make_pair(0, 0);

// Model path
static const char* modelNanosuitPath = "D:/home/Prism/resources/model/nanosuit/nanosuit.obj";
static const char* modelBackpackPath = "D:/home/Prism/resources/model/backpack/backpack.obj";
static const char* modelMonkeyPath = "D:/home/Prism/resources/model/monkey/untitled.obj";

static glm::vec3 ModelPosition = glm::vec3(0.0f, 1.0f, 0.0f);
static glm::mat4 ModelTransform = glm::scale(glm::mat4(1.0f), glm::vec3(0.6f));
static float ModelSpeed = 15.0f;

static glm::vec3 EnemyPosition = glm::vec3(10.0f, 1.0f, 10.0f);
static glm::mat4 EnemyTransform = glm::mat4(1.0f);
static float EnemySpeed = 15.0f;

static glm::vec3 lightPos = glm::vec3(0, 40.0f, 0);
static float lightColor[3] { 0.98f, 0.98f, 0.82f };
// yellow 0.93f, 0.9f, 0.55f

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

static glm::mat4 groundModelTransform = glm::scale(glm::mat4(1.0f), glm::vec3(20.0f));
static glm::vec3 groundModelPosition = glm::vec3(0.0f, 0.0f, 0.0f);

static glm::mat4 backWallTransform =  glm::scale(glm::mat4(1.0f), glm::vec3(20.0f)) 
                                   * glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(1, 0, 0));
static glm::vec3 backWallPosition = glm::vec3(0.0f, 20.0f, -20.0f);

static glm::mat4 leftWallTransform = glm::scale(glm::mat4(1.0f), glm::vec3(20.0f)) 
                                   * glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(0, 0, 1));
static glm::vec3 leftWallPosition = glm::vec3(20.0f, 20.0f, 0.0f);

static glm::mat4 rightWallTransform = glm::scale(glm::mat4(1.0f), glm::vec3(20.0f)) 
                                   * glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0, 0, 1));
static glm::vec3 rightWallPosition = glm::vec3(-20.0f, 20.0f, 0.0f);

static glm::mat4 lightCubeTransform = glm::mat4(1.0f);
static glm::vec3 lightCubePosition = lightPos;

static glm::mat4 HPModelTransform = glm::scale(glm::mat4(1.0f), glm::vec3(20.0f, 0.5f, 1.0f))
                                * glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(1, 0, 0));
static glm::vec3 HPModelPosition = glm::vec3(0.0f, 5.0f, -19.0f);

static bool isAnimating = false;

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

static std::vector<std::shared_ptr<Texture>> TextureBase;

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
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) {
        camera->ProcessMouseMovement(0, -5);
    }
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) {
        camera->ProcessMouseMovement(0, 5);
    }
    if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS) {
        camera->ProcessMouseMovement(-5, 0);
    }
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
        camera->ProcessMouseMovement(5, 0);
    }
}