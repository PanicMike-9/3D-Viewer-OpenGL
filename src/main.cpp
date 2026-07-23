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

// draw elements
void render()
{
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0); 
}

// exit window with q or esc keys
void exit_window(GLFWwindow* window)
{
    if(glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

#if 0
// unique cube position for each new cube
std::vector<glm::vec3> cube_position 
{
    //          x      y      z
    glm::vec3( 0.0f,  0.0f,  0.0f), // origin as z = 0
    glm::vec3( 1.0f, -1.0f,  0.0f),
    glm::vec3(-1.0f, -1.0f,  0.0f),
    glm::vec3(-1.0f,  1.0f,  0.0f),
    glm::vec3( 1.0f,  1.0f,  0.0f),

    glm::vec3( 0.0f,  0.0f,  2.0f), // origin as z = 2
    glm::vec3( 1.0f, -1.0f,  2.0f),
    glm::vec3(-1.0f, -1.0f,  2.0f),
    glm::vec3(-1.0f,  1.0f,  2.0f),
    glm::vec3( 1.0f,  1.0f,  2.0f),
};

// create cubes at different positions
void create_cubes(Shader &shader, glm::mat4 view, glm::mat4 projection)
{
    for(const glm::vec3 &pos : cube_position)
    {
        // create model matrix
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, pos);

        // set shader values
        shader.set_mat4("model", model);
        shader.set_mat4("view", view);
        shader.set_mat4("projection", projection);

        // render each cube
        render();
    }
}
#endif

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

    #if 0
    float cube_vertices[]
    {
    //    x      y     z       r     g     b
        -0.5f, -0.5f, 0.5f,   1.0f, 0.0f, 0.0f, 
         0.5f, -0.5f, 0.5f,   0.8f, 0.2f, 1.0f, 
         0.5f,  0.5f, 0.5f,   0.5f, 0.6f, 1.0f,
        -0.5f,  0.5f, 0.5f,   0.7f, 0.4f, 1.0f,

        -0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0.5f,
         0.5f, -0.5f, -0.5f,  0.5f, 1.0f, 0.6f,
         0.5f,  0.5f, -0.5f,  0.8f, 1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.4f, 1.0f, 0.6f,
    };

    GLuint cube_indices[]
    {
        // front face
        0, 1, 2,
        0, 2, 3,

        // back face
        4, 5, 6,
        4, 6, 7,

        // right face
        1, 5, 6,
        1, 6, 2,

        // left face
        0, 4, 7,
        0, 7, 3,

        // top face
        2, 3, 7,
        2, 7, 6,

        // bottom face
        0, 1, 5,
        0, 5, 4,
    };

    // vertex buffer object (VBO)
    GLuint vbo = 0;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices) * sizeof(float), cube_vertices, GL_STATIC_DRAW);

    // vertex array object(VAO)
    GLuint vao = 0;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // color
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // element buffer objec (EBO) for cube
    GLuint ebo = 0;
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);

    // unbind
    glBindVertexArray(0);
    #endif

    //stbi_set_flip_vertically_on_load(true);

    // enable depth test to view in 3d
    glEnable(GL_DEPTH_TEST);

    // shader code files
    Shader shader("shaders/v_shader.vert", "shaders/f_shader.frag");

    Model model_3d("assets/models/suzanne.gltf");

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

    // main window loop
    while (!glfwWindowShouldClose(window))
    {
        // calculate delta time 
        float current_frame = static_cast<float>(glfwGetTime()); 
        delta_time = current_frame - last_frame;
        last_frame = current_frame;

        exit_window(window); // exit window using q or esc key

        // window background color
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f); 
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // return the view values
        glm::mat4 view = camera.get_view_matrix();
        glm::mat4 projection = camera.get_projection_matrix(win_aspect);

        camera_controller(window, camera, delta_time);

        shader.use(); // shader code

        shader.set_mat4("view", view);
        shader.set_mat4("projection", projection);

        // render the model
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // center the model
        model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f)); // scale the model down

        shader.set_mat4("model", model);
        model_3d.draw(shader);
 
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