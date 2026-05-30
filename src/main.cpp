#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "shader.hpp"

#include <iostream>

const int win_height = 800;
const int win_width = 600;

// close windowf with ESC or Q key
void process_input(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) || glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}

int main()
{
    glfwInit();

    GLFWwindow* window = glfwCreateWindow(win_height, win_width, "3D Object Viewer", NULL, NULL);

    // window creation failure
    if(!window)
    {
        std::cerr << "Failed to create window!\n";
        glfwTerminate();
        return -1;
    }

    std::cout << "Window creation success!\n";

    // create window
    glfwMakeContextCurrent(window);

    // glad fail check
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD!\n";
        return -1;
    }

    // cube vertices
    //float vertices[] =
    //{
    //    // x     y     z
    //    -0.5,  -0.5,  0.5,
    //     0.5,  -0.5,  0.5,
    //     0.5,   0.5,  0.5,
    //    -0.5,   0.5,  0.5,

    //    -0.5,  -0.5, -0.5,
    //     0.5,  -0.5, -0.5,
    //     0.5,   0.5, -0.5,
    //    -0.5,   0.5, -0.5
    //};

    // cube indices
    //unsigned int indices[]
    //{
    //    0, 1, 2,
    //    0, 2, 3,

    //    4, 5, 6,
    //    4, 6, 7,

    //    1, 5, 6,
    //    1, 6, 2,

    //    0, 4, 7,
    //    0, 7, 3,

    //    2, 3, 7,
    //    2, 7, 6,

    //    0, 1, 5,
    //    0, 5, 4,
    //};

    float vertices[]
    {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f,
    };

    // vertex array object
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // vertex buffer object
    unsigned int VBO;
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // element buffer object
    //unsigned int EBO;
    //glGenBuffers(1, &EBO);

    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    std::cout << "GLAD load success!\n";
    
    // custom window color
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f); 

    // load shader files
    Shader shader("shader/v_shader.vert", "shader/f_shader.frag");

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    // main window loop
    while(!glfwWindowShouldClose(window))
    {
        // close window using q or esc
        process_input(window);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // use shader code
        shader.use();

        // draw 
        glBindVertexArray(VAO);
        //glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    std::cout << "Exit main loop!\n";

    return 0;
}