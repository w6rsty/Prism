#include "renderer.hpp"
#include "GLFW/glfw3.h"
#include <memory>
#include <string>
#include "glm/fwd.hpp"
#include "pecs.hpp"
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

    _first_mouse = false;
    _delta_time = 0.0f;
    _last_time = 0.0f;
    _last_x = _width / 2.0f;
    _last_y = _height / 2.0f;

    _camera = nullptr;
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
    if (_window) {
        glfwDestroyWindow(_window);
    }
    if (_camera) {
        delete  _camera;
    }
    glfwTerminate();
}

bool Renderer::init() {
    if (_camera == nullptr) {
        printf("\x1b[31;1m[Renderer initialization Failed]No camera!\x1b[0m\n");
        return false;
    }

    if (!glfwInit()) {
        printf("\x1b[31;1m Failed to initialize GLFW\n\x1b[0m\n");
        return false;
    }

    glfwWindowHint(GLFW_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    _window = glfwCreateWindow(_width, _height, _name, NULL, NULL);
    if (_window == nullptr) {
        printf("\x1b[31;1mFaild to create GLFW window\n\x1b[0m\n");
        return false;
    } else {
        printf("\x1b[32;1mSuccessfully create window '%s' (%d,%d)\n\x1b[0m", _name, _width, _height);
    }
    glfwMakeContextCurrent(_window);
    glfwSwapInterval(1);


    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        printf("\x1b[31;1mFailed to initialize OpenGL context\n\x1b[0m\n");
        return false;
    }

    glfwSetWindowUserPointer(_window, this);
    auto mouse_func = [](GLFWwindow* window, double xpos, double ypos) {
        static_cast<Renderer*>(glfwGetWindowUserPointer(window))->mouse_callback(xpos, ypos);
    };
    auto scroll_func = [](GLFWwindow* window, double xoffset, double yoffset) {
        static_cast<Renderer*>(glfwGetWindowUserPointer(window))->scroll_callback(xoffset, yoffset);
    };
    glfwSetCursorPosCallback(_window, mouse_func);
    glfwSetScrollCallback(_window, scroll_func);

    _ui = new UI(this); 

    initShaders();
    initModels();
    axisShader();
    shaders_["ground"] = ShaderProgram {
        "ground",
        ShaderProgramType::Plane,
        std::make_shared<Shader>(planeVertexPath, planeFragPath)
    };

    _texs["ground_diff"] = new Texture("D:/home/Prism/resources/img/gray_rocks_diff.jpg");
    _texs["ground_nor"] = new Texture("D:/home/Prism/resources/img/gray_rocks_nor.jpg");
    ground_ = std::make_shared<pmodel::Plane>();

    _lightColor[0] = 1.0f;
    _lightColor[1] = 1.0f;
    _lightColor[2] = 1.0f;
    _lightPos[0] = 0.0f;
    _lightPos[1] = 3.0f;
    _lightPos[2] = 0.0f;   

    _ui->imguiInit();

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

    processInput(_window);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    _ui->imguiLayout();

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
                model.second.ptr->Draw(shader);
            }
        }
    }
    {
        auto& shader = *shaders_["ground"].shader;
        shader.Bind();
        shader.setUniformMat4f("proj_matrix", _vMat);
        shader.setUniformMat4f("view_matrix", view);
        glm::mat4 mMat = glm::scale(glm::mat4(1.0f), glm::vec3(20, 0, 20));
        shader.setUniform3f("viewPos", cpos.x, cpos.y, cpos.z);
        shader.setUniform3f("light.pos", _lightPos[0], _lightPos[1], _lightPos[2]);
        shader.setUniform3f("light.color", _lightColor[0], _lightColor[1], _lightColor[2]);
        shader.setUniformMat4f("model_matrix", mMat);
        ground_->Draw(shader, _texs["ground_diff"], _texs["ground_nor"]);
    }


    if (_axis_mode) {
        drawAxis(view);
    }

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(_window);
    glfwPollEvents();   
}

void Renderer::processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        _camera->ProcessKeyboard(FORWARD, _delta_time);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        _camera->ProcessKeyboard(BACKWARD, _delta_time);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        _camera->ProcessKeyboard(LEFT, _delta_time);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        _camera->ProcessKeyboard(RIGHT, _delta_time);
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        _camera->ProcessKeyboard(UP, _delta_time);
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
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
        _last_x = xpos;
        _last_y = ypos;
        _first_mouse = false;
    }
    float xoffset = xpos - _last_x;
    float yoffset = _last_y - ypos;

    _last_x = xpos;
    _last_y = ypos;
    if (glfwGetKey(_window, GLFW_KEY_SPACE) == GLFW_PRESS) {
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
            std::make_shared<pmodel::Model>(info.modelPath, info.flip),
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