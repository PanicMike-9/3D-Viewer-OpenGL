#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

constexpr const int win_height = 800;
constexpr const int win_width = 600;

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
    float vertices[] =
    {
        // x     y     z
        -0.5,  -0.5,  0.5,
         0.5,  -0.5,  0.5,
         0.5,   0.5,  0.5,
        -0.5,   0.5,  0.5,

        -0.5,  -0.5, -0.5,
         0.5,  -0.5, -0.5,
         0.5,   0.5, -0.5,
        -0.5,   0.5, -0.5
    };

    std::cout << "GLAD load success!\n";
    
    // custom window color
    glClearColor(0.5f, 0.5f, 0.9f, 1.0f); 

    // main window loop
    while(!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        process_input(window);
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    std::cout << "Exit main loop!\n";

    return 0;
}