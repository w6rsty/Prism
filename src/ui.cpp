#include "ui.hpp"

#include "imgui.h"

#include "glsl_loader.hpp"
#include "renderer.hpp"
#include "shader.hpp"

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

    ImGui_ImplGlfw_InitForOpenGL( _rd->_window, true);
    const char* glsl_version = "#version 460";
    ImGui_ImplOpenGL3_Init(glsl_version);
}

void UI::imguiLayout() {
    imguiMainTabBar();
    if (_rd->_show_editor) imguiGLSLEditor();
    if ( _rd->_show_demo) ImGui::ShowDemoWindow();
}

void UI::initEditor() {
    _rd->_is_editing = false;
    _rd->_current_shader_src = ShaderType::None;
    memset(_rd->_editor_buffer, 0, sizeof(char) *  EDITOR_BUFFER_SIZE);
    _rd->_editor_buffer[0] = '\0';
}

void UI::imguiGLSLEditor() {
    float tab_height = ImGui::GetTextLineHeightWithSpacing();
    ImGui::SetNextWindowPos(ImVec2((float)_rd->_width / 2.0, tab_height));
    ImGui::SetNextWindowSize(ImVec2((float)_rd->_width / 2.0, (float)_rd->_height / 2.0));
    ImGui::Begin("GLSL Editor", NULL, ImGuiWindowFlags_NoResize);

    static int program_current  = 0;
    const char* names[] = {"pure", "geo", "color"};
    if (ImGui::Combo("Program", &program_current, names, std::size(names))) {
        _rd->_shader_program = names[program_current];
    }

    if (_rd->_use_color) {
        imguiColorSelector(_rd->_color);
    }

    static int item_current = 0;
    ImGui::Combo("File", &item_current, _rd->_shader_sources.data(), _rd->_shader_sources.size()); UINEXT
    if (item_current != 0) {
        _rd->_is_editing = true;
        unsigned int size = sizeof(char) * EDITOR_BUFFER_SIZE;
        loadShaderSource(_rd->_shader_sources[item_current], _rd->_editor_buffer, size);
        if (ImGui::Button("Save")) {
            printf("%s\n", _rd->_editor_buffer);
            writeShaderSource(_rd->_shader_sources[item_current], _rd->_editor_buffer);
        }

        ImGui::InputTextMultiline(
            "##source",
            _rd->_editor_buffer,
            sizeof(char) * EDITOR_BUFFER_SIZE,
            ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * 30)
        );
    }
    
    if (ImGui::IsItemActive()) {
       _rd-> _is_editing = true;
    } else {
        _rd->_is_editing = false;
    }

    ImGui::End();
}

void UI::imguiMainTabBar() {
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("Menu")) {
            if (ImGui::MenuItem("quit")) {
                glfwSetWindowShouldClose(_rd->_window, true);
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
            if (ImGui::MenuItem("GLSL Editor", _rd->_show_editor ? "*" : "")) {
                _rd->toggle(&_rd->_show_editor);
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
            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }
}

 void UI::imguiColorSelector(float* color) {
    ImGui::ColorEdit3("", _rd->_color);
 }