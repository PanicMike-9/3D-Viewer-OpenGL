#include <glad/glad.h>
#include <GLFW/glfw3.h>

// include glm for openGL math
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

#include "shader.hpp"

constexpr const int win_width = 800;
constexpr const int win_height = 600;

constexpr const double PI = 3.141592653589793; 

// press q key to close window
void process_input(GLFWwindow *window)
{
  if(glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
      glfwSetWindowShouldClose(window, true);
}

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

  // black color window
  glClearColor(0.1f, 0.0f, 0.1f, 1.0f); 

  // triangle vertex data points
  // (x, y, z, r, g, b) in-order data points on the plane and color
  float tri_vertices[] = 
  {
    0.0f,  0.5f, 0.0f,  1.0f, 0.0f, 0.0f,
    0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,
   -0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f,
  };

  float cube_vertices[]
  {
    -0.5f, -0.5f, 0.5f,
     0.5f, -0.5f, 0.5f,
     0.5f,  0.5f, 0.5f,
    -0.5f,  0.5f, 0.5f,

    -0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
    -0.5f,  0.5f, -0.5f,
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
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  // color
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(3 * sizeof(float)));
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

  // main window loop
  while(!glfwWindowShouldClose(window))
  {
    process_input(window);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shader.use(); // shader code

    // cube matrices
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), 
                                 glm::vec3(0.0f, 0.0f, 0.0f), 
                                 glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f/600.0f, 0.1f, 100.0f);

    // set values
    shader.set_mat4("model", model);
    shader.set_mat4("view", view);
    shader.set_mat4("projection", projection);

    glBindVertexArray(vao); // use vertex array obj

    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // terminate glfw
  glfwTerminate();

  std::cout << "Exited Main loop!\n";

  return 0;
}