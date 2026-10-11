#pragma once

#include <GLFW/glfw3.h>

#include "imgui.h"

namespace ui
{

void init_gui_window(GLFWwindow* main_window);
void start_gui_window_frame();
void create_gui_window(const char* gui_win_name, ImVec4& gui_clear_color, 
                    const char* button_text);
void render_gui_window();
void shutdown_gui_window();

} // namespace ui