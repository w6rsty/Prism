#pragma once

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "ui.hpp"
#include <memory>
#include <string>
#include <unordered_map>
#include "render/model.hpp"
#include "render/shader.hpp"
#include "render/camera.hpp"
#include "render/texture.hpp"
#include "render/drawable.hpp"
#include "geo/geometry.hpp"
#include "render/vertex_array.hpp"
#include "render/index_buffer.hpp"
#include "render/vertex_buffer.hpp"
#include "render/shader_program.hpp"

#define UINEXT ImGui::SameLine();
#define UIDIVIDER ImGui::Separator();

#define axisVertexPath "D:/home/Prism/resources/shader/axis_vertex.glsl"
#define axisFragPath "D:/home/Prism/resources/shader/axis_frag.glsl"

#define texPath "D:/home/Prism/resources/img/image.png"
#define fontPath1 "D:/home/Prism/resources/font/JetBrainsMonoNerdFontMono-Regular.ttf"
#define fontPath2 "D:/home/Prism/resources/font/JetBrainsMonoNerdFontMono-SemiBold.ttf"

namespace prism {

struct CreateModelInfo {
    const char* name;
    const char* modelPath;
    bool flip;
    glm::mat4 mMat = glm::mat4(1.0f);
};

struct CreateShaderInfo {
    const char* name;
    const char* vsPath;
    const char* fsPath;
};

class UI;

class Renderer {
private:
    // 窗口信息
    int                 _width;
    int                 _height;
    float               _aspect;
    const char*         _name;
    GLFWwindow*         window_;
    // 摄像机属性
    Camera*             _camera;
    // cursor position
    bool                _first_mouse;
    glm::mat4           _vMat;

    bool                _frame_mode;
    // frame render time
    float               _delta_time;
    float               _last_time;
    bool                enable_vsync_;
    std::unordered_map<std::string, VertexArray*>   _vaos;
    std::unordered_map<std::string, VertexBuffer*>  _vbos;
    std::unordered_map<std::string, IndexBuffer*>   _ibos;
    std::unordered_map<std::string, Texture*>       _texs;

    std::vector<CreateModelInfo>                    createModelInfos_; 
    std::vector<CreateShaderInfo>                   createShaderInfos_; 

    glm::mat4          globalTransform_ = glm::mat4(1.0f);

    Drawable* ptr_;

    std::unordered_map<std::string, ShaderProgram>  shaders_;

    struct ModelRenderInfo {
        std::shared_ptr<Model> ptr;
        glm::mat4 mMat;
    };

    std::unordered_map<std::string, ModelRenderInfo>  models_;

    float               _lightColor[3];
    float               _lightPos[3];

    bool                _axis_mode;
    bool                _should_cull;
    // ===== ImGui ====
    friend class UI;
    std::unique_ptr<UI> ui_;
    bool                _show_demo;
    bool                show_debug_;

    enum Theme {
        Light =  0,
        Dark,
        Classic,
    };

    int                 _theme = Theme::Dark;
    std::unordered_map<std::string, ImFont*> _fonts;
    // ===== Signal =====
    bool keys_[GLFW_KEY_LAST] = { false };
    float last_x_;
    float last_y_;
public:
    int ticker = {};
public:
    Renderer(int w, int h, const char* name);
    ~Renderer();

    bool init();
    void render();

    inline int width() const { return _width; }
    inline int height() const { return _height; }
    inline void setWindow(GLFWwindow* window) { window_ = window; }
    inline GLFWwindow* getWindow() const { return window_; }
    inline void setCamera(Camera* camera) { _camera = camera; }

    void createModel(CreateModelInfo info);
    void createShader(CreateShaderInfo info);

    inline void setGlobalTransform(glm::mat4 mat) { globalTransform_ = mat; }
private:
    void initModels();
    void initShaders();

    void toggleFrameMode();
    void toggleVSync();
    void toggle(bool* value);
    void axisShader();
    void drawAxis(glm::mat4& view);
    void mouse_callback(double xposIn, double yposIn);
    void scroll_callback( double xoffset, double yoffset);
    void key_callback(int key, int scancode, int action, int mods);
};

} // namespace prism