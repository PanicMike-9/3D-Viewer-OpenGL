#include "camera.hpp"

// constructor
Camera::Camera()
{
    update_camera_vectors();
}

// matrix function
glm::mat4 Camera::get_view_matrix()
{
    return glm::lookAt(m_position, m_position + m_front, m_up);
}

// get projection
glm::mat4 Camera::get_projection_matrix()
{
    return glm::perspective(glm::radians(m_fov), 800.0f/600.0f, 0.1f, 100.0f);
}

// process keyboard input for camera movement
void Camera::process_keyboard(camera_movement direction, float delta_time)
{
    // walk around camera with WASD
    float velocity = m_speed * delta_time;

    // press left shift to increase camera speed by 2x
    if(direction == camera_movement::SPEED_UP)
    {
        velocity *= 2.0f;
    }
    // press W for camera forward
    if(direction == camera_movement::FORWARD)
    {
        m_position += velocity * m_front;
    }
    // press S for camera backward
    if(direction == camera_movement::BACKWARD)
    {
        m_position -= velocity * m_front;
    }
    // press A for camera left
    if(direction == camera_movement::LEFT)
    {
        m_position -= m_right * velocity;
    }
    // press D for camera right
    if(direction == camera_movement::RIGHT)
    {
        m_position += m_right * velocity;
    }
}

// use mouse for rotation and look around
void Camera::process_mouse_movement(double x_offset, double y_offset)
{
    // control sensitivity
    x_offset *= m_sensitivity;
    y_offset *= m_sensitivity;

    // add offsets values to yaw and pitch
    m_yaw   += x_offset;
    m_pitch += y_offset;

    // minimum and maximum pitch values
    float min_pitch = -89.0f; 
    float max_pitch = 89.0f;

    // clamp between min and max values
    m_pitch = glm::clamp(m_pitch, min_pitch, max_pitch);

    // update camera vectors based on new orientation
    update_camera_vectors();
}

// handle scroll wheel input for zooming
void Camera::process_scroll_wheel(double y_offset)
{
    // min max values for field of view
    float max_fov = 45.0f;
    float min_fov = 1.0f;

    // adjust field of view based on scroll wheel input
    m_fov -= static_cast<float>(y_offset);
    m_fov = glm::clamp(m_fov, min_fov, max_fov);

    update_camera_vectors();
}

// update the camera's front, right, and up vectors based on Euler angles
void Camera::update_camera_vectors()
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