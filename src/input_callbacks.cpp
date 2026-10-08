#include "input_callbacks.hpp"
#include "imgui.h"

static constexpr float WIN_WIDTH  {1280.0f};
static constexpr float WIN_HEIGHT {720.0f};
static constexpr float WIN_ASPECT {WIN_WIDTH / WIN_HEIGHT};

void exit_window(GLFWwindow* window)
{
    if(glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

//  for window resizing
void framebuffer_size_callback(GLFWwindow* window, float width, float height)
{
    glViewport(0, 0, width, height);
}

// cursor to the center of the window
static float last_x {WIN_WIDTH / 2.0f};
static float last_y {WIN_HEIGHT / 2.0f};
static bool first_mouse {true};

void mouse_callback(GLFWwindow* window, double x_pos, double y_pos)
{
    Camera* cam = static_cast<Camera*>(glfwGetWindowUserPointer(window));

    // skip camera process if ImGui wants focus
    if (ImGui::GetIO().WantCaptureMouse)
    {
        return;
    }

    // check first time receiving mouse input
    if(first_mouse)
    {
        last_x = x_pos;
        last_y = y_pos;
        first_mouse = false; 
    }

    // calculate offset movement between last frame and current frame
    float x_offset = x_pos - last_x;
    float y_offset = last_y - y_pos;

    // new cursor values
    last_x = x_pos;
    last_y = y_pos;

    cam->process_mouse_movement(x_offset, y_offset);
}

// take scroll wheel input
void scroll_callback(GLFWwindow* window, double x_offset, double y_offset)
{
    // create camera pointer
    Camera* cam = static_cast<Camera*>(glfwGetWindowUserPointer(window));

    cam->process_scroll_wheel(y_offset);
}

// control camera with WASD
void camera_controller(GLFWwindow* window, Camera& camera, const float delta_time)
{
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        camera.process_keyboard(CameraMovement::FORWARD, delta_time);
    }

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        camera.process_keyboard(CameraMovement::LEFT, delta_time);
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        camera.process_keyboard(CameraMovement::BACKWARD, delta_time);
    }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        camera.process_keyboard(CameraMovement::RIGHT, delta_time);
    }

    // press 'c' to swtich to FPS and back to FLY
    static bool c_key_was_pressed {false};
    bool c_key_is_pressed {(glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)};

    if (c_key_is_pressed && !c_key_was_pressed)
    {
        if (camera.mode == CameraMode::FPS)
        {
            camera.mode = CameraMode::FLY;
        }
        else 
        {
            camera.mode = CameraMode::FPS;
            camera.position.y = 3.0f;
        }
    }
    c_key_was_pressed = c_key_is_pressed;
}
