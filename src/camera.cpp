#include "camera.hpp"

// constructor
Camera::Camera()
{
    update_camera_vectors();
}

// get view matrix
glm::mat4 Camera::get_view_matrix()
{
    return glm::lookAt(position, position + front, up);
}

// get projection
glm::mat4 Camera::get_projection_matrix(float aspect_ratio)
{
    return glm::perspective(glm::radians(fov), aspect_ratio, 0.1f, 100.0f);
}

// process keyboard input for camera movement
void Camera::process_keyboard(CameraMovement direction, float delta_time)
{
    // walk around camera with WASD
    float velocity {speed * delta_time};

    // new variables for FPS mode
    glm::vec3 move_front = front;
    glm::vec3 move_right = right;

    // x & z ground plane
    if (mode == CameraMode::FPS)
    {
        move_front = glm::normalize(glm::vec3(front.x, 0.0f, front.z));
        move_right = glm::normalize(glm::vec3(right.x, 0.0f, right.z));
    }

    // movement
    if (direction == CameraMovement::FORWARD)
    {
        position += move_front * velocity;
    }
    if (direction == CameraMovement::BACKWARD)
    {
        position -= move_front * velocity;
    }
    if (direction == CameraMovement::LEFT)
    {
        position -= move_right * velocity;
    }
    if (direction == CameraMovement::RIGHT)
    {
        position += move_right * velocity;
    }

    if (mode == CameraMode::FPS)
    {
        position.y = 3.0f;
    }
}

// use mouse for rotation and look around
void Camera::process_mouse_movement(double x_offset, double y_offset)
{
    // control sensitivity
    x_offset *= sensitivity;
    y_offset *= sensitivity;

    // add offsets values to yaw and pitch
    yaw   += x_offset;
    pitch += y_offset;

    // minimum and maximum pitch values
    float min_pitch {-89.0f};
    float max_pitch {89.0f};

    // clamp between min and max values
    pitch = glm::clamp(pitch, min_pitch, max_pitch);

    // update camera vectors based on new orientation
    update_camera_vectors();
}

// handle scroll wheel input for zooming
void Camera::process_scroll_wheel(double y_offset)
{
    // min max values for field of view
    float max_fov {45.0f};
    float min_fov {1.0f};

    // adjust field of view based on scroll wheel input
    fov -= static_cast<float>(y_offset);
    fov = glm::clamp(fov, min_fov, max_fov);

    update_camera_vectors();
}

// update the camera's front, right, and up vectors based on Euler angles
void Camera::update_camera_vectors()
{
    // look at target
    glm::vec3 direction;

    // spherical angles to 3D cartesian direction vector (x, y, z)
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));                       
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

    // directions for the camera
    front = glm::normalize(direction);
    right = glm::normalize(glm::cross(front, world_up));
    up    = glm::normalize(glm::cross(right, front));
}