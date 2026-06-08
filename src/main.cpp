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

// shader code
#include "shader.hpp"

// window height and width values
constexpr const int win_width = 800;
constexpr const int win_height = 600;

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

void update_transform_matrices(Shader& shader, float& angle)
{
    // cube matrices
    glm::mat4 model = glm::mat4(1.0f);

    glm::mat4 model_2 = glm::mat4(1.0f);
    model_2 = glm::translate(model_2, glm::vec3(2.0f, 0.0f, 0.0f));

    glm::mat4 view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), 
                                 glm::vec3(0.0f, 0.0f, 0.0f), 
                                 glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 projection = glm::perspective(glm::radians(90.0f), 800.0f/600.0f, 0.1f, 100.0f);

    // rotates the cube                             
    model = glm::rotate(model, angle, glm::vec3(1.0f, 0.5f, 1.0f));

    // set values and draw
    shader.set_mat4("model", model);
    render();

    shader.set_mat4("model", model_2);
    render();

    shader.set_mat4("view", view);
    shader.set_mat4("projection", projection);
}

// rotate, increase and decrease speed and angle of the cube vertices
void update_input(GLFWwindow *window, float& angle, float& speed)
{
    // increase speed using left and right arrow keys
    if(glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    {
      speed += 0.01f;

      if(speed > 2.5f)
      {
        speed = 2.5f;
      }

      std::cout << "Speed: " << std::fixed << std::setprecision(2) << speed << '\n';
    }
    else if(glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
    {
      speed -= 0.01f;

      if(speed <= 0.0f)
      {
        speed = 0.0f;
      }

      std::cout << "Speed: " << std::fixed << std::setprecision(2) << speed << '\n';
    }

    // increment angle by 0.01 for slower rotation speed
    if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
      angle += 0.01f * speed;
    }
    if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
      angle += -0.01f * speed;
    }
}

void orbit_camera(Shader& shader)
{
    // camera 
    glm::vec3 camera_pos = glm::vec3(0.0f, 0.0f, 3.0f); 
    glm::vec3 camera_target = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 camera_direction = glm::normalize(camera_pos - camera_target);

    // right camera axis
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 camera_right = glm::normalize(glm::cross(up, camera_direction));

    // up camera axis
    glm::vec3 camera_up = glm::cross(camera_direction, camera_right);

    // camera orbit
    float radius = 10.0f;
    float cam_x = sin(glfwGetTime()) * radius;
    float cam_z = cos(glfwGetTime()) * radius;

    // camera model, view and projection(camera MVP)
    glm::mat4 camera_model = glm::mat4(1.0f);
    glm::mat4 camera_projection = glm::perspective(glm::radians(45.0f), 800.0f/600.0f, 0.1f, 100.0f);
    glm::mat4 camera_view = glm::lookAt(glm::vec3(cam_x, 0.0f, cam_z),  //position
                                        glm::vec3(0.0f, 0.0f, 0.0f),  // target
                                        glm::vec3(0.0f, 1.0f, 0.0f)); // up vector

    // set shader values
    shader.set_mat4("model", camera_model);
    shader.set_mat4("projection", camera_projection);
    shader.set_mat4("view", camera_view);
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
//   x      y     z        r     g     b
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
  glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

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

  // enable depth test
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

    orbit_camera(shader);

    update_input(window, angle, speed); // cube movement logic

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