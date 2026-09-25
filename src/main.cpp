// glad and glfw libraries
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// OpenGL math or glm
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// basic C++ libs
#include <iostream>
#include <print>
#include <string>
#include <array>

// header files
#include "shader.hpp"
#include "camera.hpp"
#include "model.hpp"
#include "mesh.hpp"
#include "light_manager.hpp"

// window height, width and aspect ratio values
constexpr float WIN_WIDTH = 1920.0f;
constexpr float WIN_HEIGHT = 1080.0f;
constexpr float WIN_ASPECT = WIN_WIDTH / WIN_HEIGHT;

// exit window with q or esc keys
inline void exit_window(GLFWwindow* window)
{
    if(glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

//  for window resizing
inline void framebuffer_size_callback(GLFWwindow* window, float width, float height)
{
    glViewport(0, 0, width, height);
}

// cursor to the center of the window
float last_x = WIN_WIDTH / 2.0f;
float last_y = WIN_HEIGHT / 2.0f;
bool first_mouse = true;

inline void mouse_callback(GLFWwindow* window, double x_pos, double y_pos)
{
    Camera* cam = static_cast<Camera*>(glfwGetWindowUserPointer(window));

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
inline void scroll_callback(GLFWwindow* window, double x_offset, double y_offset)
{
    // create camera pointer
    Camera* cam = static_cast<Camera*>(glfwGetWindowUserPointer(window));

    cam->process_scroll_wheel(y_offset);
}

// control camera with WASD
inline void camera_controller(GLFWwindow* window, Camera& camera, const float delta_time)
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

constexpr std::size_t MAX_POINT_LIGHTS {4};
constexpr std::array<glm::vec3, MAX_POINT_LIGHTS> point_lights_pos 
{    
    glm::vec3{ 4.0f,  3.0f, 0.0f},
    glm::vec3{-4.0f,  3.0f, 0.0f},
    glm::vec3{ 4.0f,  3.0f, 1.0f},
    glm::vec3{-4.0f,  3.0f, 1.0f},
};

inline void managed_point_light(LightManager& light_manager)
{
    // point light colors
    constexpr glm::vec3 neon_cyan     {0.0f, 0.9f, 1.0f};
    constexpr glm::vec3 hot_magenta   {1.0f, 0.0f, 0.6f};
    constexpr glm::vec3 sunset_orange {1.0f, 0.35f, 0.1f};
    constexpr glm::vec3 royal_purple  {0.45f, 0.2f, 0.7f};

    // for grey monochromatic ambient light
    // constexpr glm::vec3 pl_ambient {0.02f, 0.02f, 0.02f};

    constexpr float pl_constant  {1.0f};
    constexpr float pl_linear    {0.09f};
    constexpr float pl_quadratic {0.032f};

    constexpr std::array<glm::vec3, MAX_POINT_LIGHTS> point_lights_colors 
    {
        neon_cyan, hot_magenta, sunset_orange, royal_purple
    };

    // running the loop 4 times, for 4 point lights and 4 colors
    for (std::size_t i {0}; i < MAX_POINT_LIGHTS; ++i)
    {
        PointLight light;

        light.position = point_lights_pos[i];

        // light.ambient  = pl_ambient; // monochrome, no tint
        light.ambient  = point_lights_colors[i] * 0.05f;
        light.diffuse  = point_lights_colors[i];
        light.specular = point_lights_colors[i];

        light.constant  = pl_constant;
        light.linear    = pl_linear;
        light.quadratic = pl_quadratic;

        light_manager.add_point_light(light);
    }
}

constexpr std::size_t MAX_SPOT_LIGHTS {2};
constexpr std::array<glm::vec3, MAX_SPOT_LIGHTS> spot_lights_pos
{
    glm::vec3{ 2.0f, 6.0f, 0.0f},
    glm::vec3{-2.0f, 6.0f, 0.0f},
};

inline void managed_spot_light(LightManager& light_manager)
{
    constexpr glm::vec3 royal_purple {0.45f, 0.2f, 0.7f};
    constexpr glm::vec3 gold_amber    {1.0f, 0.6f, 0.2f};

    constexpr glm::vec3 sl_ambient   {0.0f, 0.0f, 0.0f};
    constexpr glm::vec3 sl_direction {0.0f, -1.0f, 0.0f};

    constexpr float sl_constant  {1.0f};
    constexpr float sl_linear    {0.09f};
    constexpr float sl_quadratic {0.032f};

    float sl_cut_off       {glm::cos(glm::radians(12.5f))};
    float sl_outer_cut_off {glm::cos(glm::radians(17.5f))};

    constexpr std::array<glm::vec3, MAX_SPOT_LIGHTS> spot_lights_colors {royal_purple, gold_amber};

    for (std::size_t i {0}; i < MAX_SPOT_LIGHTS; ++i)
    {
        SpotLight light;
        light.position  = spot_lights_pos[i];
        light.direction = sl_direction;

        light.ambient  = sl_ambient;
        light.diffuse  = spot_lights_colors[i];
        light.specular = spot_lights_colors[i];

        light.constant  = sl_constant;
        light.linear    = sl_linear;
        light.quadratic = sl_quadratic;

        light.cut_off       = sl_cut_off;
        light.outer_cut_off = sl_outer_cut_off;

        light_manager.add_spot_light(light);
    }
}

inline void window_background_color()
{
    // rgb phases
    constexpr float red_bgc    {0.1f};
    constexpr float green_bgc  {0.1f};
    constexpr float blue_bgc   {0.12f};
    glClearColor(red_bgc, green_bgc, blue_bgc, 1.0f); 
}

LightManager light_manager;

int main()
{
    glfwInit();

    GLFWwindow* window = glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, "LightManager Testing", nullptr, nullptr);

    if (!window)
    {
        std::cerr << "COULD NOT CREATE WINDOW!\n";
        glfwTerminate();
        return -1;
    }

    std::cout << "WINDOW CREATED!\n";

    // create window
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "COULD NOT INITIALIZE GLAD\n";
        return -1;
    }

    std::cout << "GLAD LOADED!\n";

    // for consistent frame-rate 
    float delta_time {0.0f};
    float last_frame {0.0f};

    // camera class obj
    Camera camera;

    glfwSetWindowUserPointer(window, &camera);

    // hide cursor when window is in focus
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // set cursor position and call the mouse_callback
    glfwSetCursorPosCallback(window, mouse_callback);

    // set scroll wheel callback for zoom
    glfwSetScrollCallback(window, scroll_callback);

    // enable depth test to view in 3d
    glEnable(GL_DEPTH_TEST);

    window_background_color();

    // shader code files
    Shader shader("shaders/v_shader.vert", "shaders/f_shader.frag");

    // load assets
    Model hum_model_1("assets/models/low_poly_human/scene.gltf");
    Model hum_model_2("assets/models/low_poly_human/scene.gltf");
    Model floor("assets/models/checkered_tile_floor/scene.gltf");

    // main render loop
    while (!glfwWindowShouldClose(window))
    {
        // calculate delta time 
        float current_frame {static_cast<float>(glfwGetTime())}; 
        delta_time = current_frame - last_frame;
        last_frame = current_frame;

        exit_window(window); 

        // clear screen's color memory to background color
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // return the view values
        glm::mat4 view = camera.get_view_matrix();
        glm::mat4 projection = camera.get_projection_matrix(WIN_ASPECT);

        // shader code (make sure to call at the top)
        shader.use(); 

        camera_controller(window, camera, delta_time);

        shader.set_mat4("view", view);
        shader.set_mat4("projection", projection);

        shader.set_vec3("view_pos", camera.position);

        // manage light casters
        managed_point_light(light_manager);
        managed_spot_light(light_manager);

        // directional light (disabled in LightManager)
        light_manager.update_shader_uniforms(shader);

        // material properties
        constexpr int mat_diffuse  {0};
        constexpr int mat_specular {1};
        constexpr float shininess  {128.0f};

        shader.set_int("material.texture_diffuse1", mat_diffuse);
        shader.set_int("material.texture_specular1", mat_specular);
        shader.set_float("material.shine", shininess);

        // floor flat plane texture
        glm::mat4 floor_plane = glm::mat4(1.0f);
        floor_plane = glm::translate(floor_plane, glm::vec3(0.0f));
        floor_plane = glm::scale(floor_plane, glm::vec3(0.25f));

        // rotate the plane -90 radians on x axis
        floor_plane = glm::rotate(floor_plane, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

        shader.set_mat4("model", floor_plane);
        floor.draw(shader);

        // render the model poly-human 1
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-2.0f, 0.0f, 0.0f)); 
        model = glm::scale(model, glm::vec3(0.2f)); // model size/scale

        // model rotation speed
        float rotate_by {static_cast<float>(glfwGetTime()) * 0.8f};

        // rotate the model on y-axis, for presentation 
        model = glm::rotate(model, rotate_by, glm::vec3(0.0f, 1.0f, 0.0f));

        shader.set_mat4("model", model);
        hum_model_1.draw(shader);

        // render the model poly-human 2
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f)); 
        model = glm::scale(model, glm::vec3(0.2f)); 

        // rotate the model on y-axis, for presentation
        model = glm::rotate(model, rotate_by, glm::vec3(0.0f, 1.0f, 0.0f));

        shader.set_mat4("model", model);
        hum_model_2.draw(shader);

        framebuffer_size_callback(window, WIN_WIDTH, WIN_HEIGHT);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // destroy window & terminate
    glfwDestroyWindow(window);
    std::cout << "glfwDestroyWindow(window) EXECUTED\n";

    glfwTerminate();
    std::cout << "glfwTerminate() EXECUTED\n";

    return 0;
}