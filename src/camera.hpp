#pragma once

// glm libs
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

struct Camera
{
    Camera();

    glm::vec3 m_position = glm::vec3(0.0f, 0.0f, 3.0f); 
    glm::vec3 m_front    = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 m_up       = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 m_right;
    glm::vec3 m_world_up;
    
    // camera options
    float m_speed = 5.5f;
    float m_fov   = 45.0f;

    // for consistent timing 
    float m_delta_time = 0.0f;
    float m_last_frame = 0.0f;

    // euler angles
    float m_yaw        = -90.0f;
    float m_pitch      = 0.0f;
    bool m_first_mouse = true;
};