#pragma once

// glm libs
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

enum class CameraMode
{
    FPS,
    FLY,
};

enum class CameraMovement
{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
};

class Camera
{
    public:
        // camera motion position
        glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f); 
        glm::vec3 front =    glm::vec3(0.0f, 0.0f, -1.0f);
        glm::vec3 up =       glm::vec3(0.0f, 1.0f, 0.0f);
        glm::vec3 right =    glm::vec3(1.0f, 0.0f, 0.0f);
        glm::vec3 world_up = glm::vec3(0.0f, 1.0f, 0.0f);
        
        // camera details
        float speed = 5.5f;
        float fov =   45.0f;

        // euler angles
        float yaw =        -90.0f;
        float pitch =       0.0f;
        float sensitivity = 0.1f;

        // constructor
        Camera();

        // matrix functions
        glm::mat4 get_view_matrix();
        glm::mat4 get_projection_matrix(float aspect_ratio);

        // process keyboard for camera movement
        void process_keyboard(CameraMovement direction, float delta_time);

        // process mouse movement
        void process_mouse_movement(double x_offset, double y_offset);

        // handle scroll wheel input for zooming
        void process_scroll_wheel(double y_offset); 

    private:
        void update_camera_vectors();
};