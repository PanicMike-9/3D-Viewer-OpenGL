#pragma once

#include <GLFW/glfw3.h>

#include "imgui.h"
#include "camera.hpp"

constexpr float WIN_WIDTH  {1280.0f};
constexpr float WIN_HEIGHT {720.0f};
constexpr float WIN_ASPECT {WIN_WIDTH / WIN_HEIGHT};

inline void exit_window(GLFWwindow* window);
inline void framebuffer_size_callback(GLFWwindow* window, float width, float height);

// cursor to the center of the window
inline float last_x = WIN_WIDTH / 2.0f;
inline float last_y = WIN_HEIGHT / 2.0f;
inline bool first_mouse = true;

inline void mouse_callback(GLFWwindow* window, double x_pos, double y_pos);
inline void scroll_callback(GLFWwindow* window, double x_offset, double y_offset);
inline void camera_controller(GLFWwindow* window, Camera& camera, const float delta_time);