#include "engine/imgui/imgui_layer.hpp"

#include "imgui.h"
#include "engine/core.hpp"
#include "engine/application.hpp"
#include "engine/event/event.hpp"
#include "engine/event/key_event.hpp"
#include "engine/event/mouse_event.hpp"
#include "engine/log.hpp"
#include "GLFW/glfw3.h"

namespace prism {

ImGuiLayer::ImGuiLayer()
    : Layer("ImGui Layer")
{
    Window& window = Application::GetInstance()->GetWindow();
    ui_ = std::make_unique<UI>(window.GetWidth(), window.GetHeight(), &delta_time_);
}

ImGuiLayer::~ImGuiLayer() {

}

void ImGuiLayer::OnAttach() {
    PRISM_CORE_INFO("[Dear ImGui] {0}", ImGui::GetVersion());
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    ImGuiIO& io = ImGui::GetIO();
    io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
	io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.Fonts->AddFontFromFileTTF(FontPath, 30.0f);

    io.KeyMap[ImGuiKey_Tab] = GLFW_KEY_TAB;
    io.KeyMap[ImGuiKey_LeftArrow] = GLFW_KEY_LEFT;
    io.KeyMap[ImGuiKey_RightArrow] = GLFW_KEY_RIGHT;
    io.KeyMap[ImGuiKey_UpArrow] = GLFW_KEY_UP;
    io.KeyMap[ImGuiKey_DownArrow] = GLFW_KEY_DOWN;
    io.KeyMap[ImGuiKey_PageUp] = GLFW_KEY_PAGE_UP;
    io.KeyMap[ImGuiKey_PageDown] = GLFW_KEY_PAGE_DOWN;
    io.KeyMap[ImGuiKey_Home] = GLFW_KEY_HOME;
    io.KeyMap[ImGuiKey_End] = GLFW_KEY_END;
    io.KeyMap[ImGuiKey_Insert] = GLFW_KEY_INSERT;
    io.KeyMap[ImGuiKey_Delete] = GLFW_KEY_DELETE;
    io.KeyMap[ImGuiKey_Backspace] = GLFW_KEY_BACKSPACE;
    io.KeyMap[ImGuiKey_Space] = GLFW_KEY_SPACE;
    io.KeyMap[ImGuiKey_Enter] = GLFW_KEY_ENTER;
    io.KeyMap[ImGuiKey_Escape] = GLFW_KEY_ESCAPE;
    io.KeyMap[ImGuiKey_A] = GLFW_KEY_A;
    io.KeyMap[ImGuiKey_C] = GLFW_KEY_C;
    io.KeyMap[ImGuiKey_V] = GLFW_KEY_V;
    io.KeyMap[ImGuiKey_X] = GLFW_KEY_X;
    io.KeyMap[ImGuiKey_Y] = GLFW_KEY_Y;
    io.KeyMap[ImGuiKey_Z] = GLFW_KEY_Z;

    ImGui_ImplOpenGL3_Init("#version 460");
}
void ImGuiLayer::OnDetach() {

}
void ImGuiLayer::OnUpdate() {
		ImGuiIO& io = ImGui::GetIO();
		Application* app = Application::GetInstance();
		io.DisplaySize = ImVec2(app->GetWindow().GetWidth(), app->GetWindow().GetHeight());

		float timeNow = static_cast<float>(glfwGetTime());
		io.DeltaTime = time_ > 0.0f ? (timeNow - time_) : (1.0f / 60.0f);
        delta_time_ = io.DeltaTime;
		time_ = timeNow;

		ImGui_ImplOpenGL3_NewFrame();
		ImGui::NewFrame();

        ui_->imguiLayout();
        app->GetWindow().SetSync(ui_->enable_vsync);

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
void ImGuiLayer::OnEvent(Event& event) {
    EventDispatcher dispatcher(event);
    dispatcher.Dispatch<MouseButtonPressedEvent>(PRISM_BIND_EVENT_FN(ImGuiLayer::OnMouseButtonPressedEvent));
    dispatcher.Dispatch<MouseButtonReleasedEvent>(PRISM_BIND_EVENT_FN(ImGuiLayer::OnMouseButtonReleasedEvent));
    dispatcher.Dispatch<MouseMovedEvent>(PRISM_BIND_EVENT_FN(ImGuiLayer::OnMouseMovedEvent));
    dispatcher.Dispatch<MouseScrolledEvent>(PRISM_BIND_EVENT_FN(ImGuiLayer::OnMouseScrolledEvent));
    dispatcher.Dispatch<KeyPressedEvent>(PRISM_BIND_EVENT_FN(ImGuiLayer::OnKeyPresssedEvent));
    dispatcher.Dispatch<KeyReleasedEvent>(PRISM_BIND_EVENT_FN(ImGuiLayer::OnKeyReleasedEvent));
    dispatcher.Dispatch<KeyTypedEvent>(PRISM_BIND_EVENT_FN(ImGuiLayer::OnKeyTypedEvent));
}

bool ImGuiLayer::OnMouseButtonPressedEvent(MouseButtonEvent& event) {
	ImGuiIO& io = ImGui::GetIO();
    io.MouseDown[event.GetMouseButton()] = true;

	return false;
}

bool ImGuiLayer::OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& event) {
    ImGuiIO& io = ImGui::GetIO();
    io.MouseDown[event.GetMouseButton()] = false;
    return false;
}

bool  ImGuiLayer::OnMouseMovedEvent(MouseMovedEvent& event) {
    ImGuiIO& io = ImGui::GetIO();
    io.MousePos = ImVec2(event.GetX(), event.GetY());
    return false;
}

bool ImGuiLayer::OnMouseScrolledEvent(MouseScrolledEvent& event) {
    ImGuiIO& io = ImGui::GetIO();
    io.MouseWheelH += event.GetXOffset();
    io.MouseWheel += event.GetYOffset();
    return false;
}

bool ImGuiLayer::OnKeyPresssedEvent(KeyPressedEvent& event) {
    ImGuiIO& io = ImGui::GetIO();
    io.KeysDown[event.GetKeyCode()] = true;

    io.KeyCtrl = io.KeysDown[GLFW_KEY_LEFT_CONTROL] || io.KeysDown[GLFW_KEY_RIGHT_CONTROL];
    io.KeyShift = io.KeysDown[GLFW_KEY_LEFT_SHIFT] || io.KeysDown[GLFW_KEY_RIGHT_SHIFT];
    io.KeyAlt = io.KeysDown[GLFW_KEY_LEFT_ALT] || io.KeysDown[GLFW_KEY_RIGHT_ALT];
    io.KeySuper = io.KeysDown[GLFW_KEY_LEFT_SUPER] || io.KeysDown[GLFW_KEY_RIGHT_SUPER];
    return false;
}

bool ImGuiLayer::OnKeyReleasedEvent(KeyReleasedEvent& event) {
    ImGuiIO& io = ImGui::GetIO();
    io.KeysDown[event.GetKeyCode()] = false;
    return false;
}

bool ImGuiLayer::OnKeyTypedEvent(KeyTypedEvent& event) {
    ImGuiIO& io = ImGui::GetIO();
    int keycode = event.GetKeyCode();
    if (keycode > 0 && keycode < 0x10000)
        io.AddInputCharacter((unsigned short)keycode);
    return false;
}

bool ImGuiLayer::OnWindowResizeEvent(WindowResizeEvent& event) {
    ImGuiIO& io = ImGui::GetIO();
    io.DisplaySize = ImVec2(event.GetWidth(), event.GetHeight());
    io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);
    glViewport(0.0f, 0.0f, event.GetWidth(), event.GetHeight());
    return false;
}


} // namespace prism