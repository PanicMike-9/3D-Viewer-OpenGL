#pragma once

// glm libs
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// GLFW for window 
#include <GLFW/glfw3.h>

class Camera
{
    public:
        // camera motion position
        glm::vec3 m_position = glm::vec3(0.0f, 0.0f, 3.0f); 
        glm::vec3 m_front    = glm::vec3(0.0f, 0.0f, -1.0f);
        glm::vec3 m_up       = glm::vec3(0.0f, 1.0f, 0.0f);
        glm::vec3 m_right;
        glm::vec3 m_world_up = glm::vec3(0.0f, 1.0f, 0.0f);
        
        // camera options
        float m_speed = 5.5f;
        float m_fov   = 45.0f;

        // euler angles
        float m_yaw        = -90.0f;
        float m_pitch      = 0.0f;
        bool m_first_mouse = true;

        // cursor to the center of the window
        float m_last_x = win_width / 2.0f;
        float m_last_y = win_height / 2.0f;

        // constructor
        Camera();

        // matrix functions
        glm::mat4 get_view_matrix()
        {
            return glm::lookAt(m_position, m_position + m_front, m_up);
        }

        // process keyboard input for camera movement
        void process_keyboard(GLFWwindow* window, float delta_time)
        {
            // walk around camera with WASD
            float camera_speed = 5.5f * delta_time;

            // press left shift to increase camera speed by 2x
            if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
            {
                camera_speed *= 2.0f;
            }
            // press W for camera forward
            if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            {
                m_position += camera_speed * m_front;
            }
            // press S for camera backward
            if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            {
                m_position -= camera_speed * m_front;
            }
            // press A for camera left
            if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            {
                m_position -= m_right * camera_speed;
            }
            // press D for camera right
            if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            {
                m_position += m_right * camera_speed;
            }
        }

        // use mouse for rotation and look around
        void mouse_callback(GLFWwindow *window, double x_pos, double y_pos)
        {
            // check if this is the first time receiving mouse input
            if(m_first_mouse)
            {
                m_last_x = x_pos;
                m_last_y = y_pos;
                m_first_mouse = false; 
            }

            // calculate offset movement between last frame and current frame
            float x_offset = x_pos - m_last_x;
            float y_offset = m_last_y - y_pos;

            // new cursor values
            m_last_x = x_pos;
            m_last_y = y_pos;

            // control sensitivity
            const float sensitivity = 0.1f;
            x_offset *= sensitivity;
            y_offset *= sensitivity;

            // add offsets values to yaw and pitch
            m_yaw   += x_offset;
            m_pitch += y_offset;

            // minimum and maximum pitch values
            float min_pitch = -89.0f; 
            float max_pitch = 89.0f;

            // clamp between min and max values
            m_pitch = glm::clamp(m_pitch, min_pitch, max_pitch);
        }

    private:
        // update the camera's front, right, and up vectors based on Euler angles
        void update_camera_vectors()
        {
            // look at target
            glm::vec3 direction;

            // spherical angles to 3D cartesian direction vector (x, y, z)
            direction.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
            direction.y = sin(glm::radians(m_pitch));
            direction.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
            m_front = glm::normalize(direction);
            m_right = glm::normalize(glm::cross(m_front, m_world_up));
            m_up = glm::normalize(glm::cross(m_right, m_front));
        }
        // update the camera's front, right, and up vectors based on Euler angles
        void update_camera_vectors()
        {
            // look at target
            glm::vec3 direction;

            // spherical angles to 3D cartesian direction vector (x, y, z)
            direction.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
            direction.y = sin(glm::radians(m_pitch));
            direction.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
            m_front = glm::normalize(direction);
            m_right = glm::normalize(glm::cross(m_front, m_world_up));
            m_up = glm::normalize(glm::cross(m_right, m_front));
        }
}; 