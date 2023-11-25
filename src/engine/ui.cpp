#include "engine/ui.hpp"

#include "GLFW/glfw3.h"
#include "imgui.h"
#include "engine/renderer.hpp"
#include "render/shader.hpp"

namespace prism {

UI::UI(Renderer* rd) {
    _rd = rd;
}

void UI::imguiInit() {
    printf("[Dear ImGui] %s\n", ImGui::GetVersion());
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    _rd->_fonts["global"] = io.Fonts->AddFontFromFileTTF(fontPath1, 30.0f);
    io.FontDefault = _rd->_fonts["global"];
    _rd->_fonts["display"] = io.Fonts->AddFontFromFileTTF(fontPath2, 60.0f);

    switch (_rd->_theme) {
        case 0: ImGui::StyleColorsLight(); break;
        case 1: ImGui::StyleColorsDark(); break;
        case 2: ImGui::StyleColorsClassic(); break;
    }

    ImGui_ImplGlfw_InitForOpenGL( _rd->window_, true);
    const char* glsl_version = "#version 460";
    ImGui_ImplOpenGL3_Init(glsl_version);
}

void UI::imguiLayout() {
    imguiMainTabBar();
    if (_rd->_show_demo) ImGui::ShowDemoWindow();
    if (_rd->show_debug_) imguiDebugPanel();
}


void UI::imguiDebugPanel() {
    float tab_height = ImGui::GetTextLineHeightWithSpacing();
    ImGui::SetNextWindowPos(ImVec2(0, tab_height));
    ImGui::SetNextWindowSize(ImVec2((float)_rd->_width / 3.0, (float)_rd->_height));
    ImGui::Begin("Debug", NULL, ImGuiWindowFlags_NoResize);

    ImGui::Text("Delta time: %.1fms", _rd->_delta_time * 1000.0f);
    ImGui::Text("FPS: %d", int(1.0f / _rd->_delta_time));
    if (ImGui::Checkbox("VSync", &_rd->enable_vsync_)) {
        _rd->toggleVSync();
    }

    auto& cpos = _rd->_camera->Position;
    ImGui::Text("cam pos: (%.3f %.3f %.3f)", cpos.x, cpos.y, cpos.z);
    ImGui::SliderFloat3("light.pos", _rd->_lightPos, -10.0f, 10.0f);
    ImGui::ColorEdit3("light.color", _rd->_lightColor);

    ImGui::End();
}

void UI::imguiMainTabBar() {
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("Menu")) {
            if (ImGui::MenuItem("quit")) {
                glfwSetWindowShouldClose(_rd->window_, true);
            }
            if (ImGui::BeginMenu("Preference")) {
                if (ImGui::BeginMenu("Theme")) {
                    if (ImGui::MenuItem("Light")) {
                        ImGui::StyleColorsLight();
                    }
                    if (ImGui::MenuItem("Dark")) {
                        ImGui::StyleColorsDark();
                    }
                    if (ImGui::MenuItem("Classic")) {
                        ImGui::StyleColorsClassic();
                    }
                    ImGui::EndMenu();
                }
                ImGui::EndMenu();
            }
            ImGui::EndMenu();
        }

        if(ImGui::BeginMenu("View")) {
            if (ImGui::MenuItem("Demo")) {
                _rd->toggle(&_rd->_show_demo);
            }
            if (ImGui::MenuItem("Frame mode", _rd->_frame_mode ? "*" : "")) {
                _rd->toggleFrameMode();
            }
            if (ImGui::MenuItem("Axis mode", _rd->_axis_mode ? "*" : "")) {
                _rd->toggle(&_rd->_axis_mode);
            }
            if (ImGui::MenuItem("Face Cull", _rd->_should_cull ? "*" : "")) {
                _rd->toggle(&_rd->_should_cull);
            }
            if (ImGui::MenuItem("Show debug", _rd->show_debug_ ? "*" : "")) {
                _rd->toggle(&_rd->show_debug_);
            }
            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }
}

} // namespace prism