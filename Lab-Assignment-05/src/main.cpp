#include "glad.h"
#include "glfw3.h"

#include <iostream>
#include <cmath>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// -----------------------------
// Vertex Shader
// -----------------------------
const char* vertexShaderSource =
"#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"    gl_Position = vec4(aPos, 1.0);\n"
"}\0";

// -----------------------------
// Fragment Shader
// -----------------------------
const char* fragmentShaderSource =
"#version 330 core\n"
"out vec4 FragColor;\n"
"uniform vec4 ourColor;\n"
"void main()\n"
"{\n"
"    FragColor = ourColor;\n"
"}\n\0";

int main()
{
    // -----------------------------
    // Initialize GLFW
    // -----------------------------
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // -----------------------------
    // Create Window
    // -----------------------------
    // Replace YOUR_ID with your own ID
    GLFWwindow* window =
        glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "0432410005101029", NULL, NULL);

    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window"
                  << std::endl;

        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    glfwSetFramebufferSizeCallback(
        window,
        framebuffer_size_callback
    );

    // -----------------------------
    // Initialize GLAD
    // -----------------------------
    if (!gladLoadGLLoader(
            (GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD"
                  << std::endl;

        return -1;
    }

    // -----------------------------
    // Vertex Shader
    // -----------------------------
    unsigned int vertexShader =
        glCreateShader(GL_VERTEX_SHADER);

    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);

    glCompileShader(vertexShader);

    int success;
    char infoLog[512];

    glGetShaderiv(
        vertexShader,
        GL_COMPILE_STATUS,
        &success
    );

    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);

        std::cout << "Vertex Shader Error:\n"
                  << infoLog << std::endl;
    }

    // -----------------------------
    // Fragment Shader
    // -----------------------------
    unsigned int fragmentShader =
        glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);

    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS,  &success);

    if (!success)
    {
        glGetShaderInfoLog(fragmentShader,512,NULL,  infoLog);

        std::cout << "Fragment Shader Error:\n"
                  << infoLog << std::endl;
    }

    // -----------------------------
    // Shader Program
    // -----------------------------
    unsigned int shaderProgram =
        glCreateProgram();

    glAttachShader(
        shaderProgram,
        vertexShader
    );

    glAttachShader(
        shaderProgram,
        fragmentShader
    );

    glLinkProgram(shaderProgram);

    glGetProgramiv(
        shaderProgram,
        GL_LINK_STATUS,
        &success
    );

    if (!success)
    {
        glGetProgramInfoLog(
            shaderProgram,
            512,
            NULL,
            infoLog
        );

        std::cout << "Shader Program Error:\n"
                  << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // -----------------------------
    // Upside-Down Triangle
    // -----------------------------
    float vertices[] =
    {

        -0.5f,  0.5f, 0.0f,   // Top-left
         0.5f,  0.5f, 0.0f,   // Top-right
         0.0f, -0.5f, 0.0f    // Bottom
    };

    unsigned int VBO;
    unsigned int VAO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(
        GL_ARRAY_BUFFER,
        VBO
    );

    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(vertices),
        vertices,
        GL_STATIC_DRAW
    );

    glVertexAttribPointer(0,  3,  GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glEnableVertexAttribArray(0);

    // -----------------------------
    // Render Loop
    // -----------------------------
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        // -----------------------------
        // Black Background
        // -----------------------------
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

        glClear(GL_COLOR_BUFFER_BIT);

        // Use Shader Program
        glUseProgram(shaderProgram);

        // -----------------------------
        // Cyan <-> White Animation
        // -----------------------------
        float timeValue = glfwGetTime();

        /*
            sin() gives values between -1 and +1.

            We convert it to 0 -> 1:

            sin(time)       : -1 -> +1
            sin(time) + 1   :  0 -> 2
            / 2              :  0 -> 1
        */

        float value =
            (sin(timeValue) / 2.0f) + 0.5f;

        

        float red = value;
        float green = 1.0f;
        float blue = 1.0f;

        int colorLocation =
            glGetUniformLocation(
                shaderProgram,
                "ourColor"
            );

        glUniform4f(colorLocation, red, green, blue, 1.0f);

        // -----------------------------
        // Draw Triangle
        // -----------------------------
        glBindVertexArray(VAO);

        glDrawArrays( GL_TRIANGLES, 0, 3);

        // -----------------------------
        // Display
        // -----------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // -----------------------------
    // Cleanup
    // -----------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    glfwTerminate();

    return 0;
}

// -----------------------------
// Keyboard Input
// -----------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey( window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(
            window,
            true
        );
    }
}

// -----------------------------
// Window Resize
// -----------------------------
void framebuffer_size_callback(
    GLFWwindow* window,
    int width,
    int height)
{
    glViewport(
        0,
        0,
        width,
        height
    );
}