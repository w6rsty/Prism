#include "engine/ui.hpp"

#include "GLFW/glfw3.h"
#include "imgui.h"
#include "engine/renderer.hpp"
#include "render/shader.hpp"
#include "config/config.hpp"

namespace prism {

UI::UI(int width, int height, float* time)
    : windowWidth_(width), windowHeight_(height), delta_time_(time) 
{}

UI::~UI() {}

void UI::imguiLayout() {
    imguiMainTabBar();
    if (show_demo_) {
		ImGui::ShowDemoWindow(&show_demo_);
    }
    if (show_debug_) {
        imguiDebugPanel();
    }
}

void UI::imguiDebugPanel() {
    float tab_height = ImGui::GetTextLineHeightWithSpacing();
    ImGui::SetNextWindowPos(ImVec2(0, tab_height));
    ImGui::SetNextWindowSize(ImVec2((float)windowWidth_ / 3.0, (float)windowHeight_));
    ImGui::Begin("Debug", NULL, ImGuiWindowFlags_NoResize);

    ImGui::Text("Delta time: %.1fms", getTime() * 1000.0f);
    ImGui::Text("FPS: %d", int(1.0f / getTime()));
    ImGui::Checkbox("VSync", &enable_vsync);
    ImGui::Checkbox("Frame Mode", &show_frame_);

    ImGui::End();
}

void UI::imguiMainTabBar() {
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("Menu")) {
            if (ImGui::MenuItem("quit")) {

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
                show_demo_ = !show_demo_;
            }
            if (ImGui::MenuItem("Debug")) {
                show_debug_ = !show_debug_;
            }
            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }
}

} // namespace prism