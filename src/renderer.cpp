#include "renderer.hpp"
#include "GLFW/glfw3.h"
#include "geo.hpp"
#include "shader_program.hpp"
#include "texture.hpp"
#include <string>

Renderer::Renderer(int w, int h, const char* name)
    : _width(w), _height(h), _name(name)
{
    _aspect = (float)_width / (float)_height;
    _frame_mode = false;
    _axis_mode = false;
    _show_demo = false;
    _show_editor = false;
    _modify_presicion = false;
    _should_cull = false;
    _front_face = GL_CCW;
    _shader_program = "pure";

    _first_mouse = false;
    _delta_time = 0.0f;
    _last_time = 0.0f;
    _last_x = _width / 2.0f;
    _last_y = _height / 2.0f;

    _color[0] = 1.0;
    _color[1] = 1.0;
    _color[2] = 1.0;
    _use_color = false;

    _camera = new Camera(glm::vec3(0.0, 0.0, 3.0f));
}

Renderer::~Renderer() {
    _geos.clear();
    _vaos.clear();
    _vbos.clear();
    _ibos.clear();
    _texs.clear();
    _shaders.clear();
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

    axisShader();

    {
        Geo* cube = new Sphere();

        _geos["geos"] = cube;
        _vaos["cube"] = new VertexArray();
        _vbos["cube"] = new VertexBuffer(cube->getVertices(), cube->getSize());
        _ibos["cube"] = new IndexBuffer(cube->getIndices(), cube->getCount());

        VertexBufferLayout layout;
        layout.push_float(3);
        layout.push_float(2);
        layout.push_float(3);
        _vaos["cube"]->addBuffer(*_vbos["cube"], layout);
        
        auto tex_data = new unsigned char[2048 * 1024 * 3];
        createCheckboardTexture(tex_data, 2048, 1024, 16);
        Texture* tex = new Texture(2048, 1024, tex_data);
        
        _texs["tex"] = tex;
        
        std::string paths[] = {vertexPath, fragPath};
        _shaders["geo"] = ShaderProgram{
            .name = "geo",
            .type = ShaderProgramType::Tex_Normal,
            .shader = new Shader(paths)
        };

        _lightColor[0] = 1.0f;
        _lightColor[1] = 1.0f;
        _lightColor[2] = 1.0f;
        _lightPos[0] = 3.0f;
        _lightPos[1] = 3.0f;
        _lightPos[2] = 3.0f;   
    }

    {
        std::string paths[] = {pureVertexPath, pureFragPath};
        _shaders["pure"] = ShaderProgram {
            .name = "pure",
            .type = ShaderProgramType::Pure,
            .shader = new Shader(paths),
        };
    }
    {
        std::string paths[] = {colorVertexPath, colorFragPath};
        _shaders["color"] = ShaderProgram {
            .name = "color",
            .type = ShaderProgramType::Color,
            .shader = new Shader(paths),
        };
    }

    _ui->initEditor();
    _ui->imguiInit();
    return true;
}

void Renderer::run() {
    while (!glfwWindowShouldClose(_window)) {
        // Delta time calcualtion
        float currentFrame = static_cast<float>(glfwGetTime());
        _delta_time = currentFrame - _last_time;
        _last_time= currentFrame;

        // Buffer Clear
        glClearColor(0, 0, 0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);
        glClear(GL_DEPTH_BUFFER_BIT);
        if (_should_cull) {
            glEnable(GL_CULL_FACE);
        } else {
            glDisable(GL_CULL_FACE);
        }
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);

        processInput(_window);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        _ui->imguiLayout();


        _vMat = glm::perspective(glm::radians(_camera->Zoom), _aspect, 0.1f, 1000.0f);
        auto view = _camera->GetViewMatrix();

        {
            ShaderProgram& program = _shaders[_shader_program];
            program.shader->Bind();
            _ibos["cube"]->Bind();
            _vaos["cube"]->Bind();

            if (program.type == ShaderProgramType::Tex_Normal || program.type == ShaderProgramType::Color) {
                _texs["tex"]->Bind(0);
                program.shader->setUniform1i("samp", 0);
                program.shader->setUniform3f("lightColor", _lightColor[0], _lightColor[1], _lightColor[2]);
                program.shader->setUniform3f("lightPos", _lightPos[0], _lightPos[1], _lightPos[2]);
                program.shader->setUniform3f("viewPos", _camera->Position.x, _camera->Position.y, _camera->Position.z);
            }
            
            if (program.type == ShaderProgramType::Color) {
                _use_color = true;
                program.shader->setUniform3f("tex_color", _color[0], _color[1], _color[2]);
            } else { _use_color = false; }

            program.shader->setUniformMat4f("proj_matrix", _vMat);
            program.shader->setUniformMat4f("view_matrix", view);
            glm::mat4 mMat = glm::scale(glm::mat4(1.0f), glm::vec3(1, 1, 1));
            program.shader->setUniformMat4f("model_matrix", mMat);

            glDrawElements(GL_TRIANGLES, _geos["geos"]->getCount(), GL_UNSIGNED_INT, 0);
        }

        if (_axis_mode) {
            drawAxis(view);
        }

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(_window);
        glfwPollEvents();   
    }
}

void Renderer::processInput(GLFWwindow *window)
{
    if (!_is_editing) {
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
    std::string axis[] = {axisVertexPath, axisFragPath};
    _shaders["axis"] = ShaderProgram {
        .name = "axis",
        .type = ShaderProgramType::Axis,
        .shader = new Shader(axis),
    };
    _vbos["axis"]->Unbind();
    _vaos["axis"]->Unbind();
}

void Renderer::drawAxis(glm::mat4& view) {
    ShaderProgram& program = _shaders["axis"];
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