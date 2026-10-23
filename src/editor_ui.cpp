#include "editor_ui.hpp"

#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

namespace ui
{

void init_gui_window(GLFWwindow* main_window)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();

    // disable imgui.ini file generation
    io.IniFilename = nullptr;

    ImGui_ImplGlfw_InitForOpenGL(main_window, true);
    ImGui_ImplOpenGL3_Init("#version 410 core");
}

void start_gui_window_frame()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void create_gui_window(const char* gui_win_name, ImVec4& gui_clear_color, const char* button_text)
{
    ImGuiIO& io = ImGui::GetIO();

    // must be static & called once, not ever frame
    // static ImVec4 gui_clear_color {1.00f, 1.00f, 1.00f, 1.00f};

    ImGui::Begin(gui_win_name);
    ImGui::Text("FPS %.1f", io.Framerate);
    ImGui::Text("Add Button");
    ImGui::ColorEdit3("GUI color", (float*)&gui_clear_color);

    static bool light_on {false};
    if (ImGui::Button(button_text))
    {
        light_on = !light_on;
    }

    ImGui::SameLine();
    ImGui::Text("Value = %d", light_on);

    ImGui::End();
}

void render_gui_window()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void shutdown_gui_window()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

} // namespace ui