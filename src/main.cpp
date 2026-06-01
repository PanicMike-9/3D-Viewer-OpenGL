#include <glad/glad.h>
#include <GLFW/glfw3.h>
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
  GLFWwindow* window = glfwCreateWindow(win_width, win_height, "3d Viewer", NULL, NULL);

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

  // vertex buffer object (VBO)
  GLuint vbo = 0;
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(tri_vertices), tri_vertices, GL_STATIC_DRAW);

  // vertex array object(VAO)
  GLuint vao = 0;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, vao);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

  // position
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  // color
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  // shader code files
  Shader shader("shaders/v_shader.vert", "shaders/f_shader.frag");

  float angle = 0.0f; // angle changes

  // x and y coordinate values
  float x_offset = 0.0f;
  float y_offset = 0.0f;

  // main window loop
  while(!glfwWindowShouldClose(window))
  {
    // animation values
    float time = glfwGetTime();
    float speed = 0.01f;

    process_input(window);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shader.use(); // shader code

    // change angle with keys a and s
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
      //x_offset -= 0.01f;
      angle -= 0.01f;
    }
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
      //x_offset += 0.01f;
      angle += 0.01f;
    }

    // change position based on angle with keys w and s
    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
      x_offset += cos(angle) * speed;
      y_offset += sin(angle) * speed;
    }
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
      x_offset += -(cos(angle) * speed);
      y_offset += -(sin(angle) * speed);
    }

    // set size
    float size = 0.5f;

    shader.set_float("xOffset", x_offset); // movement in x axis
    shader.set_float("yOffset", y_offset); // movement in y axis

    shader.set_float("size", size); 

    shader.set_float("angle", angle);

    shader.set_float("u_time", glfwGetTime()); // color breath animation

    glBindVertexArray(vao); // use vertex array obj

    glDrawArrays(GL_TRIANGLES, 0, 3); // draw triangle

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // terminate glfw
  glfwTerminate();

  std::cout << "Exited Main loop!\n";

  return 0;
}