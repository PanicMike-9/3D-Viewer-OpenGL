#pragma once

// glm libs
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// camera movement directions
enum class camera_movement
{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    SPEED_UP, 
};

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
        float m_sensitivity = 0.1f;

        // constructor
        Camera();

        // matrix functions
        glm::mat4 get_view_matrix();
        glm::mat4 get_projection_matrix();

        // process keyboard for camera movement
        void process_keyboard(camera_movement direction, float delta_time);

        // process mouse movement
        void process_mouse_movement(double x_offset, double y_offset);

        // handle scroll wheel input for zooming
        void process_scroll_wheel(double y_offset); 
    

    private:
        void update_camera_vectors();
};