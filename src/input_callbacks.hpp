#pragma once

#include <GLFW/glfw3.h>

#include "camera.hpp"

void exit_window(GLFWwindow* window);
void framebuffer_size_callback(GLFWwindow* window, float width, float height);

void mouse_callback(GLFWwindow* window, double x_pos, double y_pos);
void scroll_callback(GLFWwindow* window, double x_offset, double y_offset);
void camera_controller(GLFWwindow* window, Camera& camera, const float delta_time);