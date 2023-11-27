#include "engine/renderer.hpp"
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

#define planeVertexPath "D:/home/Prism/resources/shader/plane_vertex.glsl"
#define planeFragPath "D:/home/Prism/resources/shader/plane_frag.glsl"

namespace prism {

Renderer::Renderer(int w, int h, const char* name)
    : _width(w), _height(h), _name(name)
{
    _aspect = (float)_width / (float)_height;
    show_debug_ = false;

    _first_mouse = false;
    _delta_time = 0.0f;
    _last_time = 0.0f;
    last_x_ = _width / 2.0f;
    last_y_ = _height / 2.0f;
}

Renderer::~Renderer() {
    _vaos.clear();
    _vbos.clear();
    _ibos.clear();
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

    projMat_ = glm::perspective(glm::radians(_camera->Zoom), _aspect, 0.1f, 1000.0f);
    
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

}

} // namespace prism