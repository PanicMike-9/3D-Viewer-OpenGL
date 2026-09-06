// glad and glfw libraries
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// OpenGL math or glm
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// basic C++ libs
#include <iostream>
#include <string>
#include <array>

// header files
#include "shader.hpp"
#include "camera.hpp"
#include "model.hpp"
#include "mesh.hpp"

// window height, width and aspect ratio values
constexpr float WIN_WIDTH = 1280.0f;
constexpr float WIN_HEIGHT = 720.0f;
constexpr float WIN_ASPECT = WIN_WIDTH / WIN_HEIGHT;

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
float last_x = WIN_WIDTH / 2.0f;
float last_y = WIN_HEIGHT / 2.0f;
bool first_mouse = true;

void mouse_callback(GLFWwindow* window, double x_pos, double y_pos)
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
void scroll_callback(GLFWwindow* window, double x_offset, double y_offset)
{
    // create camera pointer
    Camera* cam = static_cast<Camera*>(glfwGetWindowUserPointer(window));

    cam->process_scroll_wheel(y_offset);
}

// control camera with WASD
void camera_controller(GLFWwindow* window, Camera& camera, float delta_time)
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
}

constexpr std::array<glm::vec3, 4> point_lights_pos 
{    
    glm::vec3( 2.0f,  3.0f, 0.0f),
    glm::vec3(-2.0f,  3.0f, 0.0f),
    glm::vec3( 2.0f,  3.0f, 1.0f),
    glm::vec3(-2.0f,  3.0f, 1.0f),
};

void point_light_system(Shader& shader)
{
    // point light colors
    constexpr glm::vec3 red    = glm::vec3(1.0f, 0.0f, 0.0f);
    constexpr glm::vec3 green  = glm::vec3(0.0f, 1.0f, 0.0f);
    constexpr glm::vec3 blue   = glm::vec3(0.0f, 0.0f, 1.0f);
    constexpr glm::vec3 yellow = glm::vec3(1.0f, 1.0f, 0.0f);

    constexpr glm::vec3 pl_specular = glm::vec3(0.5f);
    constexpr glm::vec3 pl_diffuse =  glm::vec3(0.4f);
    constexpr glm::vec3 pl_ambient =  glm::vec3(0.05f);

    constexpr float pl_constant = 1.0f;
    constexpr float pl_linear = 0.009f;
    constexpr float pl_quadratic = 0.032f;

    constexpr glm::vec3 point_lights_colors[] = {red, blue, green, yellow};

    // running the loop 4 times, for 4 point lights and 4 colors
    for (int i = 0; i < 4; ++i)
    {
        std::string base = "point_lights[" + std::to_string(i) + "].";

        shader.set_vec3(base + "position", point_lights_pos[i]);

        shader.set_vec3(base + "ambient",  point_lights_colors[i] * pl_ambient); 
        shader.set_vec3(base + "diffuse",  point_lights_colors[i] * pl_diffuse);
        shader.set_vec3(base + "specular", point_lights_colors[i] * pl_specular);

        shader.set_float(base + "constant",  pl_constant);
        shader.set_float(base + "linear",    pl_linear);
        shader.set_float(base + "quadratic", pl_quadratic);
    }
}

constexpr std::array<glm::vec3, 2> spot_lights_pos
{
    glm::vec3( 2.0f,  5.0f, 1.0f),
    glm::vec3(-2.0f,  5.0f, 1.0f),
};

void spot_light_system(Shader& shader, Camera& camera)
{
    constexpr glm::vec3 red    = glm::vec3(0.5f, 0.0f, 0.0f);
    constexpr glm::vec3 yellow  = glm::vec3(0.5f, 0.5f, 0.0f);

    constexpr glm::vec3 sl_specular = glm::vec3(1.0f);
    constexpr glm::vec3 sl_diffuse =  glm::vec3(1.0f);
    constexpr glm::vec3 sl_ambient =  glm::vec3(1.0f);

    constexpr float sl_constant = 1.0f;
    constexpr float sl_linear = 0.09f;
    constexpr float sl_quadratic = 0.032f;

    float sl_cut_off = glm::cos(glm::radians(12.5f));
    float sl_outer_cut_off = glm::cos(glm::radians(17.5f));

    constexpr glm::vec3 spot_lights_colors[] = {red, yellow};

    for (int i = 0; i < 2; ++i)
    {
        std::string base = "spot_lights[" + std::to_string(i) + "].";

        shader.set_vec3(base + "position", spot_lights_pos[i]);
        shader.set_vec3(base + "direction", glm::vec3(0.0f, -1.0f, 0.0f));

        shader.set_vec3(base + "ambient",  spot_lights_colors[i] * sl_ambient); 
        shader.set_vec3(base + "diffuse",  spot_lights_colors[i] * sl_diffuse);
        shader.set_vec3(base + "specular", spot_lights_colors[i] * sl_specular);

        shader.set_float(base + "constant",  sl_constant);
        shader.set_float(base + "linear",    sl_linear);
        shader.set_float(base + "quadratic", sl_quadratic);

        shader.set_float(base + "cut_off",  sl_cut_off); 
        shader.set_float(base + "outer_cut_off", sl_outer_cut_off); 
    }
}

int main()
{
    glfwInit();

    GLFWwindow* window = glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, "Test Lighting", nullptr, nullptr);

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

    // for consistent frame-rate 
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

    // window background color (experimental sky blue)
    glClearColor(0.5f, 0.2f, 0.6f, 1.0f); 

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
        float current_frame = static_cast<float>(glfwGetTime()); 
        delta_time = current_frame - last_frame;
        last_frame = current_frame;

        exit_window(window); // exit window using q or esc key

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

        #if 0
        // multiple colors for presentation
        // light_color.x = sin(glfwGetTime() * 2.0f);
        // light_color.y = cos(glfwGetTime() * 0.7f);
        // light_color.z = sin(glfwGetTime() * 1.3f);

        // set point light
        shader.set_vec3("light.position", glm::vec3(0.0f));

        // light direction relative to camera 
        shader.set_vec3("light.direction", camera.front); 

        // light position relative to camera position
        shader.set_vec3("light.position", camera.position);

        // set spot light float values
        shader.set_float("light.cut_off", glm::cos(glm::radians(12.5f))); 
        shader.set_float("light.outer_cut_off", glm::cos(glm::radians(17.5f))); 
        shader.set_vec3("light.ambient", ambient_color);
        shader.set_vec3("light.diffuse", diffuse_color);
        shader.set_vec3("light.specular", specular_color);

        // light attenuation values
        shader.set_float("light.constant", 1.0f);
        shader.set_float("light.linear", 0.09f);
        shader.set_float("light.quadratic", 0.032f);

        // orbiting light position
        // glm::vec3 light_pos = glm::vec3(sin(glfwGetTime()) * 2.0f, 0.0f, cos(glfwGetTime()) * 2.0f);
        #endif

        shader.set_vec3("view_pos", camera.position);

        // white light color
       constexpr glm::vec3 light_color = glm::vec3(1.0f);

       constexpr glm::vec3 ambient_color  = light_color * glm::vec3(0.2f); // shadow brightness
       constexpr glm::vec3 diffuse_color  = light_color * glm::vec3(0.8f); // direct surface light 
       constexpr glm::vec3 specular_color = light_color * glm::vec3(1.0f); // brightness of shine

        #if 0 
        // turn on directional light 1
        // set directional light vectors 
        glm::vec3 light_direction = glm::normalize(glm::vec3(-0.2f, -1.0f, -0.3f));

        shader.set_vec3("dir_light.direction", light_direction);
        shader.set_vec3("dir_light.ambient",   ambient_color);
        shader.set_vec3("dir_light.diffuse",   diffuse_color);
        shader.set_vec3("dir_light.specular",  specular_color);
        #endif

        point_light_system(shader);
        // spot_light_system(shader, camera);

        // material properties
        constexpr int mat_diffuse = 0;
        constexpr int mat_specular = 1;
        constexpr float shininess = 128.0f;

        shader.set_int("material.texture_diffuse1", mat_diffuse);
        shader.set_int("material.texture_specular1", mat_specular);
        shader.set_float("material.shine", shininess);

        // floor flat plane texture
        glm::mat4 floor_plane = glm::mat4(1.0f);
        floor_plane = glm::translate(floor_plane, glm::vec3(0.0f));
        floor_plane = glm::scale(floor_plane, glm::vec3(0.25f));

        // rotate the plane 90 radians on x axis
        floor_plane = glm::rotate(floor_plane, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

        shader.set_mat4("model", floor_plane);
        floor.draw(shader);

        // render the model poly-human 1
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-2.0f, 0.0f, 0.0f)); 
        model = glm::scale(model, glm::vec3(0.2f)); // model size/scale

        // model rotation speed
        float rotate_by = glfwGetTime() * 0.8f;

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

    // destroy window
    glfwDestroyWindow(window);
    std::cout << "glfwDestroyWindow(window) executed\n";

    // terminate glfw
    glfwTerminate();
    std::cout << "glfwTerminate() executed\n";

    return 0;
}