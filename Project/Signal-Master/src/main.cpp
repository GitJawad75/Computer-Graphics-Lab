
#include "glad.h"
#include "glfw3.h"
#include <iostream>
#include <algorithm>
using namespace std;

// ================= SHADERS =================

const char* vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec2 aPos;
void main()
{
    gl_Position = vec4(aPos, 0.0, 1.0);
}
)";

const char* fragmentShaderSource = R"(
#version 330 core
uniform vec3 objectColor;
out vec4 FragColor;

void main()
{
    FragColor = vec4(objectColor, 1.0);
}
)";

// ================= GLOBAL VARIABLES =================

unsigned int shaderProgram, VAO, VBO;
int colorLocation;

// ================= RECTANGLE =================

void drawRect(float x, float y, float w, float h,
              float r, float g, float b)
{
    float vertices[] = {
        x, y, x+w, y, x+w, y+h,
        x, y, x+w, y+h, x, y+h
    };

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferSubData(GL_ARRAY_BUFFER, 0,
                    sizeof(vertices), vertices);

    glUniform3f(colorLocation, r, g, b);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

// ================= COLLISION =================

struct CarRect
{
    float x, y, w, h;
};

bool checkCollision(CarRect a, CarRect b)
{
    return a.x < b.x + b.w &&
           a.x + a.w > b.x &&
           a.y < b.y + b.h &&
           a.y + a.h > b.y;
}

bool intersectionOccupied(float carRight, float carLeft,
                          float carUp, float carDown)
{
    CarRect intersection = {-0.22f, -0.22f, 0.44f, 0.44f};

    CarRect cars[4] = {
        {carRight, -0.15f, 0.11f, 0.05f},
        {carLeft,   0.10f, 0.11f, 0.05f},
        {0.10f, carUp,    0.05f, 0.11f},
        {-0.15f, carDown, 0.05f, 0.11f}
    };

    for (int i = 0; i < 4; i++)
    {
        if (checkCollision(intersection, cars[i]))
            return true;
    }

    return false;
}

// ================= TRAFFIC LIGHT =================

void drawTrafficLight(float x, float y, bool green)
{
    drawRect(x, y, 0.055f, 0.17f,
             0.08f, 0.08f, 0.08f);

    drawRect(x+0.012f, y+0.12f, 0.031f, 0.025f,
             green ? 0.30f : 1.0f,
             green ? 0.04f : 0.05f,
             green ? 0.04f : 0.05f);

    drawRect(x+0.012f, y+0.075f, 0.031f, 0.025f,
             0.30f, 0.25f, 0.02f);

    drawRect(x+0.012f, y+0.03f, 0.031f, 0.025f,
             green ? 0.05f : 0.02f,
             green ? 1.0f : 0.25f,
             green ? 0.15f : 0.04f);
}

// ================= CAR DRAWING =================

void drawCar(float x, float y, bool horizontal,
             float r, float g, float b)
{
    if (horizontal)
    {
        drawRect(x, y, 0.11f, 0.05f, r, g, b);

        drawRect(x+0.035f, y+0.01f, 0.035f, 0.03f,
                 0.65f, 0.85f, 1.0f);
    }
    else
    {
        drawRect(x, y, 0.05f, 0.11f, r, g, b);

        drawRect(x+0.01f, y+0.04f, 0.03f, 0.035f,
                 0.65f, 0.85f, 1.0f);
    }
}

// ================= PEDESTRIAN =================

void drawPedestrian(float x, float y)
{
    drawRect(x+0.01f, y+0.035f, 0.02f, 0.02f,
             1.0f, 0.75f, 0.5f);

    drawRect(x+0.015f, y+0.005f, 0.01f, 0.03f,
             0.0f, 0.0f, 1.0f);

    drawRect(x, y-0.015f, 0.01f, 0.02f,
             0.0f, 0.0f, 0.0f);

    drawRect(x+0.025f, y-0.015f, 0.01f, 0.02f,
             0.0f, 0.0f, 0.0f);
}

// ================= MAIN =================

int main()
{
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE,
                   GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(900, 700, "Signal Master - Smart Traffic Control", NULL, NULL);

    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        cout << "GLAD loading failed!" << endl;
        glfwTerminate();
        return -1;
    }

    // ================= SHADER SETUP =================

    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // ================= VAO / VBO =================

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), NULL, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

    glEnableVertexAttribArray(0);

    colorLocation =
        glGetUniformLocation(shaderProgram, "objectColor");

    // ================= CAR POSITIONS =================

    float carRight = -1.1f;
    float carLeft  =  1.1f;
    float carUp    = -1.1f;
    float carDown  =  1.1f;

    // Track whether each car has passed its stop line
    bool rightPassedStop = false;
    bool leftPassedStop  = false;
    bool upPassedStop    = false;
    bool downPassedStop  = false;

    // ================= PEDESTRIAN =================

    float pedestrianX = -0.45f;

    bool pedestrianRequested = false;
    bool pedestrianActive = false;

    // ================= SIGNAL CONTROL =================

    bool horizontalGreen = true;

    bool key1WasDown = false;
    bool key2WasDown = false;
    bool keyPWasDown = false;

    float lastTime = (float)glfwGetTime();

    const float speed = 0.28f;
    const float pedestrianSpeed = 0.20f;

    const float rightStop = -0.365f;
    const float leftStop  =  0.255f;
    const float upStop    = -0.365f;
    const float downStop  =  0.255f;

    // ================= MAIN LOOP =================

    while (!glfwWindowShouldClose(window))
    {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        float currentTime = (float)glfwGetTime();
        float deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        if (deltaTime > 0.05f)
            deltaTime = 0.05f;

        // ================= KEYBOARD =================

        bool key1Down = glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS;
        bool key2Down = glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS;
        bool keyPDown = glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS;

        if (key1Down && !key1WasDown)
            horizontalGreen = true;

        if (key2Down && !key2WasDown)
            horizontalGreen = false;

        if (keyPDown && !keyPWasDown)
            pedestrianRequested = true;

        key1WasDown = key1Down;
        key2WasDown = key2Down;
        keyPWasDown = keyPDown;

        bool verticalGreen = !horizontalGreen;

        // ================= PEDESTRIAN REQUEST =================

        // Pedestrian starts only when the intersection is clear
        if (pedestrianRequested &&
            !pedestrianActive &&
            !intersectionOccupied(carRight, carLeft, carUp, carDown))
        {
            pedestrianActive = true;
            pedestrianRequested = false;
            pedestrianX = -0.45f;
        }

        // ================= NORMAL CAR MOVEMENT =================

        float distance = speed * deltaTime;

        bool trafficAllowed = !pedestrianActive;

        if (trafficAllowed)
        {
            // Right-moving car
            if (horizontalGreen || rightPassedStop)
            {
                carRight += distance;

                if (carRight >= rightStop)
                    rightPassedStop = true;
            }
            else
            {
                carRight = min(carRight + distance, rightStop);
            }

            // Left-moving car
            if (horizontalGreen || leftPassedStop)
            {
                carLeft -= distance;

                if (carLeft <= leftStop)
                    leftPassedStop = true;
            }
            else
            {
                carLeft = max(carLeft - distance, leftStop);
            }

            // Up-moving car
            if (verticalGreen || upPassedStop)
            {
                carUp += distance;

                if (carUp >= upStop)
                    upPassedStop = true;
            }
            else
            {
                carUp = min(carUp + distance, upStop);
            }

            // Down-moving car
            if (verticalGreen || downPassedStop)
            {
                carDown -= distance;

                if (carDown <= downStop)
                    downPassedStop = true;
            }
            else
            {
                carDown = max(carDown - distance, downStop);
            }
        }

        // ================= PEDESTRIAN MOVEMENT =================

        if (pedestrianActive)
        {
            pedestrianX += pedestrianSpeed * deltaTime;

            if (pedestrianX > 0.45f)
            {
                pedestrianX = -0.45f;
                pedestrianActive = false;
            }
        }

        // ================= RESET CARS =================

        if (carRight > 1.1f)
        {
            carRight = -1.1f;
            rightPassedStop = false;
        }

        if (carLeft < -1.1f)
        {
            carLeft = 1.1f;
            leftPassedStop = false;
        }

        if (carUp > 1.1f)
        {
            carUp = -1.1f;
            upPassedStop = false;
        }

        if (carDown < -1.1f)
        {
            carDown = 1.1f;
            downPassedStop = false;
        }

        // ================= DRAW SCENE =================

        glClearColor(0.20f, 0.48f, 0.20f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);

        // Roads
        drawRect(-1.0f, -0.22f, 2.0f, 0.44f, 0.20f, 0.21f, 0.23f);

        drawRect(-0.22f, -1.0f, 0.44f, 2.0f, 0.20f, 0.21f, 0.23f);

        // Horizontal road markings
        for (float x = -0.95f; x < 0.95f; x += 0.15f)
        {
            if (x > -0.28f && x < 0.28f)
                continue;

            drawRect(x, -0.008f, 0.07f, 0.016f, 0.95f, 0.95f, 0.85f);
        }

        // Vertical road markings
        for (float y = -0.95f; y < 0.95f; y += 0.15f)
        {
            if (y > -0.28f && y < 0.28f)
                continue;

            drawRect(-0.008f, y, 0.016f, 0.07f, 0.95f, 0.95f, 0.85f);
        }

        // Stop lines
        drawRect(-0.22f, 0.29f, 0.44f, 0.018f, 1.0f, 1.0f, 1.0f);

        drawRect(-0.22f, -0.31f, 0.44f, 0.018f, 1.0f, 1.0f, 1.0f);

        drawRect(0.29f, -0.22f, 0.018f, 0.44f, 1.0f, 1.0f, 1.0f);

        drawRect(-0.31f, -0.22f, 0.018f, 0.44f, 1.0f, 1.0f, 1.0f);

        // ================= TRAFFIC LIGHTS =================

        drawTrafficLight(-0.34f, 0.30f, horizontalGreen && !pedestrianActive);

        drawTrafficLight(0.28f, 0.30f, verticalGreen && !pedestrianActive);

        drawTrafficLight(-0.34f, -0.47f, verticalGreen && !pedestrianActive);

        drawTrafficLight(0.28f, -0.47f, horizontalGreen && !pedestrianActive);

        // ================= CARS =================

        drawCar(carRight, -0.15f, true, 0.95f, 0.15f, 0.12f);

        drawCar(carLeft, 0.10f, true, 0.15f, 0.35f, 1.0f);

        drawCar(0.10f, carUp, false, 1.0f, 0.8f, 0.1f);

        drawCar(-0.15f, carDown, false, 0.7f, 0.2f, 0.9f);

        // ================= PEDESTRIAN =================

        if (pedestrianRequested || pedestrianActive)
            drawPedestrian(pedestrianX, 0.25f);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // ================= CLEANUP =================

    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
    glDeleteProgram(shaderProgram);

    glfwTerminate();

    return 0;
}
