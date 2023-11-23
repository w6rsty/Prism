#include "renderer.hpp"
#include "GLFW/glfw3.h"
#include <memory>
#include <string>
#include "geo/geometry.hpp"
#include "render/error.hpp"
#include "render/model.hpp"
#include "render/shader.hpp"
#include "render/texture.hpp"
#include "render/index_buffer.hpp"
#include "render/vertex_array.hpp"
#include "render/vertex_buffer.hpp"
#include "render/shader_program.hpp"

#define planeVertexPath "D:/home/Prism/resources/shader/plane_vertex.glsl"
#define planeFragPath "D:/home/Prism/resources/shader/plane_frag.glsl"

namespace prism {

Renderer::Renderer(int w, int h, const char* name)
    : _width(w), _height(h), _name(name)
{
    _aspect = (float)_width / (float)_height;
    _frame_mode = false;
    _axis_mode = true;
    _should_cull = false;
    _show_demo = false;
    show_debug_ = false;
    enable_vsync_ = false;

    _first_mouse = false;
    _delta_time = 0.0f;
    _last_time = 0.0f;
    last_x_ = _width / 2.0f;
    last_y_ = _height / 2.0f;

    _lightColor[0] = 1.0f;
    _lightColor[1] = 1.0f;
    _lightColor[2] = 1.0f;
    _lightPos[0] = 0.0f;
    _lightPos[1] = 3.0f;
    _lightPos[2] = 0.0f;   

    _camera = nullptr;
    window_ = nullptr;

    if (!glfwInit()) {
        printf("\x1b[31;1m Failed to initialize GLFW\n\x1b[0m\n");
        assert(false);
    }

    glfwWindowHint(GLFW_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
}

Renderer::~Renderer() {
    _vaos.clear();
    _vbos.clear();
    _ibos.clear();
    _texs.clear();
    shaders_.clear();
    models_.clear();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    if (window_) {
        glfwDestroyWindow(window_);
    }
    if (_camera) {
        delete  _camera;
    }
}

bool Renderer::init() {
    if (!_camera) {
        printf("\x1b[31;1m[Renderer initialization Failed]No camera!\x1b[0m\n");
        return false;
    }

    if (!window_) {
        printf("\x1b[31;1m[Renderer initialization Failed]No window!\x1b[0m\n");
        return false;
    }

    glfwMakeContextCurrent(window_);
    glfwSwapInterval(0);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        printf("\x1b[31;1mFailed to initialize OpenGL context\n\x1b[0m\n");
        return false;
    }
    std::cout << "[GLAD] " << glGetString(GL_VERSION) << std::endl; 

    glfwSetWindowUserPointer(window_, this);
    auto mouse_func = [](GLFWwindow* window, double xpos, double ypos) {
        static_cast<Renderer*>(glfwGetWindowUserPointer(window))->mouse_callback(xpos, ypos);
    };
    auto scroll_func = [](GLFWwindow* window, double xoffset, double yoffset) {
        static_cast<Renderer*>(glfwGetWindowUserPointer(window))->scroll_callback(xoffset, yoffset);
    };
    auto key_func = [](GLFWwindow* window, int key, int scancode, int action, int mods) {
        static_cast<Renderer*>(glfwGetWindowUserPointer(window))->key_callback(key, scancode, action, mods);
    };
    glfwSetCursorPosCallback(window_, mouse_func);
    glfwSetScrollCallback(window_, scroll_func);
    glfwSetKeyCallback(window_, key_func);

    ui_ = std::make_unique<UI>(this); 

    initShaders();
    initModels();
    axisShader();

    ptr_ = new Sphere();

    ui_->imguiInit();

    GLCall(glEnable(GL_DEPTH_TEST));
    GLCall(glDepthFunc(GL_LESS));
    GLCall(glClearColor(0.7, 0.7, 0.7, 1.0));
    return true;
}

void Renderer::render() {
    // Delta time calcualtion
    float currentFrame = static_cast<float>(glfwGetTime());
    _delta_time = currentFrame - _last_time;
    _last_time= currentFrame;

    // Buffer Clear
    GLCall(glClear(GL_COLOR_BUFFER_BIT));
    GLCall(glClear(GL_DEPTH_BUFFER_BIT));
    if (_should_cull) {
        GLCall(glEnable(GL_CULL_FACE));
    } else {
        GLCall(glDisable(GL_CULL_FACE));
    }

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ui_->imguiLayout();

    _vMat = glm::perspective(glm::radians(_camera->Zoom), _aspect, 0.1f, 1000.0f);
    auto view = _camera->GetViewMatrix();
    auto& cpos = _camera->Position;
    {
        auto& shader = *shaders_["mesh"].shader;
        shader.Bind();

        shader.setUniformMat4f("proj_matrix", _vMat);
        shader.setUniformMat4f("view_matrix", view);
        for (const auto& model : models_) {
            if (model.second.ptr) {
                glm::mat4 mMat = globalTransform_ * model.second.mMat;
                shader.setUniformMat4f("model_matrix", mMat);
                shader.setUniform3f("viewPos", cpos.x, cpos.y, cpos.z);
                shader.setUniform3f("light.pos", _lightPos[0], _lightPos[1], _lightPos[2]);
                shader.setUniform3f("light.color", _lightColor[0], _lightColor[1], _lightColor[2]);
                model.second.ptr->onRender(shader);
            }
        }
        ptr_->onRender(shader);
    }
    
    if (_axis_mode) {
        drawAxis(view);
    }

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(window_);
    glfwPollEvents();   
}

void Renderer::key_callback(int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS) {
        keys_[scancode] = true; 
    } else if (action == GLFW_RELEASE) {
        keys_[scancode] = false;
    }
    
    if (keys_[glfwGetKeyScancode(GLFW_KEY_ESCAPE)])
        glfwSetWindowShouldClose(window_, true);
    if (keys_[glfwGetKeyScancode(GLFW_KEY_W)])
        _camera->ProcessKeyboard(FORWARD, _delta_time);
    if (keys_[glfwGetKeyScancode(GLFW_KEY_S)])
        _camera->ProcessKeyboard(BACKWARD, _delta_time);
    if (keys_[glfwGetKeyScancode(GLFW_KEY_A)])
        _camera->ProcessKeyboard(LEFT, _delta_time);
    if (keys_[glfwGetKeyScancode(GLFW_KEY_D)])
        _camera->ProcessKeyboard(RIGHT, _delta_time);
    if (keys_[glfwGetKeyScancode(GLFW_KEY_LEFT_SHIFT)])
        _camera->ProcessKeyboard(UP, _delta_time);
    if (keys_[glfwGetKeyScancode(GLFW_KEY_LEFT_CONTROL)])
        _camera->ProcessKeyboard(DOWN, _delta_time);

}


void Renderer::toggleFrameMode() {
    _frame_mode = !_frame_mode;
    if (_frame_mode) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    } else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
}

void Renderer::toggleVSync() {
    if (enable_vsync_) {
        glfwSwapInterval(1);
    } else {
        glfwSwapInterval(0);
    }
}

void Renderer::toggle(bool* value) {
    *value = !*value;
}

void Renderer::axisShader() {
    _vaos["axis"] = new VertexArray();
    float axis_vertex[] = {
        -100.0, 0.0, 0.0, 
        100.0, 0.0, 0.0, 
        0.0, -100.0, 0.0, 
        0.0, 100.0, 0.0, 
        0.0, 0.0, -100.0,
        0.0, 0.0, 100.0
    };
    _vbos["axis"] = new VertexBuffer(axis_vertex, sizeof(float) * std::size(axis_vertex));
    VertexBufferLayout axis_layout;
    axis_layout.push_float(3);
    _vaos["axis"]->addBuffer(*_vbos["axis"], axis_layout);
    shaders_["axis"] = ShaderProgram {
        "axis",
        ShaderProgramType::Axis,
        std::make_shared<Shader>(axisVertexPath, axisFragPath),
    };
    _vbos["axis"]->Unbind();
    _vaos["axis"]->Unbind();
}

void Renderer::drawAxis(glm::mat4& view) {
    ShaderProgram& program = shaders_["axis"];
    program.shader->Bind();
    _vaos["axis"]->Bind();
    program.shader->setUniformMat4f("proj_matrix", _vMat);
    program.shader->setUniformMat4f("view_matrix", view);

    glDrawArrays(GL_LINES, 0, 6);
    program.shader->Unbind();
}

void Renderer::mouse_callback(double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (_first_mouse)
    {
        last_x_ = xpos;
        last_y_ = ypos;
        _first_mouse = false;
    }
    float xoffset = xpos - last_x_;
    float yoffset = last_y_ - ypos;

    last_x_ = xpos;
    last_y_ = ypos;
    if (glfwGetKey(window_, GLFW_KEY_SPACE) == GLFW_PRESS) {
        _camera->ProcessMouseMovement(xoffset, yoffset);
    }
}
void Renderer::scroll_callback(double xoffset, double yoffset)
{
    _camera->ProcessMouseScroll(static_cast<float>(yoffset));
}

void Renderer::createModel(CreateModelInfo info) {
    createModelInfos_.emplace_back(info);
}

void Renderer::createShader(CreateShaderInfo info) {
    createShaderInfos_.emplace_back(info);
}


void Renderer::initModels() {
    for (const auto& info : createModelInfos_) {
        models_[info.name] = ModelRenderInfo {
            std::make_shared<Model>(info.modelPath, info.flip),
            info.mMat
        };
    }
    createModelInfos_.clear();
}

void Renderer::initShaders() {
    for (const auto& info : createShaderInfos_) {
        shaders_[info.name] = ShaderProgram {
            info.name,
            ShaderProgramType::Mesh,
            std::make_shared<Shader>(info.vsPath, info.fsPath)
        };
    }
    createShaderInfos_.clear();
}


} // namespace prism