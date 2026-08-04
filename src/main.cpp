// glad and glfw libraries
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// OpenGL math or glm
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// basic C++ libs
#include <iostream>
#include <vector>

// header files
#include "shader.hpp"
#include "camera.hpp"
#include "model.hpp"
#include "mesh.hpp"

//Model model;

// window height, width and aspect ratio values
constexpr const float win_width = 1200.0f;
constexpr const float win_height = 720.0f;
constexpr const float win_aspect = win_width / win_height;

constexpr const double PI = 3.141592653589793; 

// exit window with q or esc keys
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
float last_x = win_width / 2.0f;
float last_y = win_height / 2.0f;
bool first_mouse = true;

void mouse_callback(GLFWwindow* window, double x_pos, double y_pos)
{
    // create camera pointer
    Camera* cam = static_cast<Camera*>(glfwGetWindowUserPointer(window));

    // check if this is the first time receiving mouse input
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
void camera_controller(GLFWwindow* window, Camera &camera, float delta_time)
{
    // move forward with W key
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        camera.process_keyboard(camera_movement::FORWARD, delta_time);
    }

    // move forward with A key
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        camera.process_keyboard(camera_movement::LEFT, delta_time);
    }

    // move forward with S key
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        camera.process_keyboard(camera_movement::BACKWARD, delta_time);
    }

    // move forward with D key
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        camera.process_keyboard(camera_movement::RIGHT, delta_time);
    }
}

// main
int main()
{
    glfwInit();

    // declare window name and size
    GLFWwindow* window = glfwCreateWindow(win_width, win_height, "Load 3D models", NULL, NULL);

    if (!window)
    {
        std::cerr << "Could not create window!\n";
        glfwTerminate();
        return -1;
    }

    std::cout << "Window created!\n";

    // create window
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Could not initialize GLAD\n";
        return -1;
    }

    std::cout << "GLAD Loaded\n";

    //stbi_set_flip_vertically_on_load(true);

    // shader code files
    Shader shader("shaders/v_shader.vert", "shaders/f_shader.frag");

    Model hum_model_1("assets/models/low_poly_human/scene.gltf");
    Model hum_model_2("assets/models/low_poly_human/scene.gltf");

    // angle for rotation
    float angle = 0.0f;

    // speed for rotation
    float speed = 0.0f;

    // for consistent timing 
    float delta_time = 0.0f;
    float last_frame = 0.0f;

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

    // window background color
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f); 

    // main window loop
    while (!glfwWindowShouldClose(window))
    {
        // calculate delta time 
        float current_frame = static_cast<float>(glfwGetTime()); 
        delta_time = current_frame - last_frame;
        last_frame = current_frame;

        exit_window(window); // exit window using q or esc key

        // window background color
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // return the view values
        glm::mat4 view = camera.get_view_matrix();
        glm::mat4 projection = camera.get_projection_matrix(win_aspect);

        shader.use(); // shader code

        camera_controller(window, camera, delta_time);

        shader.set_mat4("view", view);
        shader.set_mat4("projection", projection);

        // ambient light color (purple)
        shader.set_vec3("light_color", glm::vec3(1.0f));

        // position the light vector
        shader.set_vec3("light_pos", glm::vec3(1.0f, 1.0f, 0.5f));
        shader.set_vec3("view_pos", camera.position);

        // render the model 1
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // center the model
        model = glm::scale(model, glm::vec3(0.2f)); // scale the model down

        shader.set_mat4("model", model);
        hum_model_1.draw(shader);

        // render the model 2
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f)); // center the model
        model = glm::scale(model, glm::vec3(0.2f)); // scale the model down

        shader.set_mat4("model", model);
        hum_model_2.draw(shader);

        framebuffer_size_callback(window, win_width, win_height);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // destroy window
    glfwDestroyWindow(window);
    std::cout << "glfwDestroyWindow(window) executed\n";

    // terminate glfw
    glfwTerminate();
    std::cout << "glfwTerminate() executed\n";

    return 0;
}