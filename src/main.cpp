// glad and glfw libraries
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// OpenGL math or glm
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// basic C++ libs
#include <iostream>
#include <iomanip>
#include <vector>

// shader code
#include "shader.hpp"

// window height, width and aspect ratio values
constexpr const float win_width = 800.0f;
constexpr const float win_height = 600.0f;
constexpr const float win_aspect = win_width / win_height;

constexpr const double PI = 3.141592653589793; 

// draw elements
void render()
{
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0); 
}

// exit window with q or esc keys
void process_input(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

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

void update_transform_matrices(Shader& shader, float& angle)
{
    // cube matrices loop
    for(const glm::vec3& pos : cube_position)
    {
        glm::mat4 model_1 = glm::mat4(1.0f);
        model_1 = glm::translate(model_1, pos);

        shader.set_mat4("model", model_1);
        render();
    }

    glm::mat4 view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), 
                                 glm::vec3(0.0f, 0.0f, 0.0f), 
                                 glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 projection = glm::perspective(glm::radians(90.0f), win_aspect, 0.1f, 100.0f);

    // set view and projection
    shader.set_mat4("view", view);
    shader.set_mat4("projection", projection);
}

// simple walk around camera for basic movement
glm::vec3 camera_position = glm::vec3(0.0f, 0.0f, 3.0f); // make global temporary

// camera values for movement
float delta_time = 0.0f;
float last_frame = 0.0f;

// Euler angles variables
float yaw = 0.0f;
float pitch = 0.0f;

void walk_around_camera(GLFWwindow* window, Shader& shader)
{
    // look at target
    glm::vec3 direction;

    // spherical angles to 3D cartesian direction vector (x, y, z)
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

    // camera values
    glm::vec3 camera_front = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 camera_up = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 camera_right = glm::normalize(glm::cross(camera_front, camera_up));

    // camera view and projection
    glm::mat4 camera_projection = glm::perspective(glm::radians(45.0f), win_aspect, 0.1f, 100.0f);
    glm::mat4 camera_view = glm::lookAt(camera_position, camera_position + camera_front, camera_up);  

    // set shader values
    shader.set_mat4("projection", camera_projection);
    shader.set_mat4("view", camera_view);

    // cast to float because glfwGetTime returns double
    float current_frame = static_cast<float>(glfwGetTime()); 

    // consistent camera movement
    delta_time = current_frame - last_frame;
    last_frame = current_frame;

    // walk around camera with WASD
    const float camera_speed = 5.5f * delta_time;

    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        camera_position += camera_speed * camera_front;
    }
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        camera_position -= camera_speed * camera_front;
    }
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        camera_position -= camera_right * camera_speed;
    }
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        camera_position += camera_right * camera_speed;
    }
}

// main
int main()
{
    glfwInit();

    // declare window name and size
    GLFWwindow* window = glfwCreateWindow(win_width, win_height, "OpenGL Cube", NULL, NULL);

    if(!window)
    {
        std::cerr << "Could not create window!\n";
        glfwTerminate();
        return -1;
    }

    std::cout << "Window created!\n";

    // create window
    glfwMakeContextCurrent(window);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Could not initialize GLAD\n";
        return -1;
    }

    std::cout << "GLAD Loaded\n";

    // window background color
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); 

    float cube_vertices[]
    {
    //    x      y     z       r     g     b
        -0.5f, -0.5f, 0.5f,   1.0f, 0.5f, 1.0f, 
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

    // shader code files
    Shader shader("shaders/v_shader.vert", "shaders/f_shader.frag");

    // enable depth test to view in 3d
    glEnable(GL_DEPTH_TEST);

    // angle for rotation
    float angle = 0.0f;

    // speed for rotation
    float speed = 0.0f;

    // main window loop
    while(!glfwWindowShouldClose(window))
    {
        process_input(window);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.use(); // shader code

        glBindVertexArray(vao); // use vertex array obj

        update_transform_matrices(shader, angle); // cube view and rotation

        walk_around_camera(window,shader);

        //update_input(window, angle, speed); // cube movement logic

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