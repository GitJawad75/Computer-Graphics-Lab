#include "glad.h"
#include "glfw3.h"

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// Window settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// Vertex Shader
const char* vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

// Cyan Fragment Shader
const char* fragmentShader1Source = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(0.0f, 1.0f, 1.0f, 1.0f);\n"
    "}\n\0";

// Magenta Fragment Shader
const char* fragmentShader2Source = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.0f, 1.0f, 1.0f);\n"
    "}\n\0";


int main()
{
    // Initialize GLFW
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // Create GLFW window
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Jawad-Ul-Karim", NULL, NULL);

    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    // Resize callback
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }


 

    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);

    unsigned int fragmentShaderCyan =
        glCreateShader(GL_FRAGMENT_SHADER);

    unsigned int fragmentShaderMagenta =
        glCreateShader(GL_FRAGMENT_SHADER);

    // Create shader programs
    unsigned int shaderProgramCyan = glCreateProgram();
    unsigned int shaderProgramMagenta = glCreateProgram();


    // Vertex Shader
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);


    // Cyan Fragment Shader
    glShaderSource(fragmentShaderCyan, 1, &fragmentShader1Source, NULL);

    glCompileShader(fragmentShaderCyan);


    // Magenta Fragment Shader
    glShaderSource(fragmentShaderMagenta, 1, &fragmentShader2Source,NULL);

    glCompileShader(fragmentShaderMagenta);



    glAttachShader(shaderProgramCyan, vertexShader);
    glAttachShader(shaderProgramCyan, fragmentShaderCyan);
    glLinkProgram(shaderProgramCyan);


    glAttachShader(shaderProgramMagenta, vertexShader);
    glAttachShader(shaderProgramMagenta, fragmentShaderMagenta);
    glLinkProgram(shaderProgramMagenta);


    

    // Cyan Square
    // The square is made using two triangles.
    float firstTriangle[] = {

        // First triangle
        0.0f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.0f,
        0.0f, 0.5f, 0.0f,

        // Second triangle
        0.0f, 0.0f, 0.0f,
        0.5f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.0f
    };


    // Magenta Triangle
    // The first two points are shared with
    // the top two corners of the square.
    float secondTriangle[] = {

        0.0f, 0.5f, 0.0f,     // Top-left corner of square
        0.5f, 0.5f, 0.0f,     // Top-right corner of square
        0.25f, 0.90f, 0.0f    // Top point of triangle
    };


    // Create VAOs and VBOs
    unsigned int VBOs[2], VAOs[2];

    glGenVertexArrays(2, VAOs);
    glGenBuffers(2, VBOs);


    glBindVertexArray(VAOs[0]);

    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);

    glBufferData(GL_ARRAY_BUFFER, sizeof(firstTriangle), firstTriangle, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glEnableVertexAttribArray(0);


    glBindVertexArray(VAOs[1]);

    glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);

    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(secondTriangle),
        secondTriangle,
        GL_STATIC_DRAW
    );

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glEnableVertexAttribArray(0);


    while (!glfwWindowShouldClose(window))
    {
        // Check keyboard input
        processInput(window);


        // White background
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

        glClear(GL_COLOR_BUFFER_BIT);


        glUseProgram(shaderProgramCyan);

        glBindVertexArray(VAOs[0]);

        glDrawArrays(GL_TRIANGLES, 0, 6);


        glUseProgram(shaderProgramMagenta);

        glBindVertexArray(VAOs[1]);

        glDrawArrays(GL_TRIANGLES, 0, 3);


        glfwSwapBuffers(window);
        glfwPollEvents();
    }



    glDeleteVertexArrays(2, VAOs);
    glDeleteBuffers(2, VBOs);

    glDeleteProgram(shaderProgramCyan);
    glDeleteProgram(shaderProgramMagenta);


    // Terminate GLFW
    glfwTerminate();

    return 0;
}


void processInput(GLFWwindow* window)
{

    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}


void framebuffer_size_callback(
    GLFWwindow* window,
    int width,
    int height
)
{
    glViewport(0, 0, width, height);
}