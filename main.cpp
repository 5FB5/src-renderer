#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 800

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <SOIL/SOIL.h>
#include <iostream>
#include <math.h>


#include "bsp.h"
#include "camera.h"
#include "shader.h"
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;
GLfloat mouseLastX = WINDOW_WIDTH / 2, mouseLastY = WINDOW_HEIGHT / 2;

bool keys[1024];
bool isSprint = false;
bool firstMouse = true;

static void glfwError(int id, const char* desc)
{
    std::cout << "[GLFW Error]: " << desc << std::endl;
}

void moveCamera()
{
    if (isSprint)
        camera.movementSpeed = 130.0f;
    else
        camera.movementSpeed = 5.f;

    if (keys[GLFW_KEY_W])
        camera.processMovement(CameraMovement::FORWARD, deltaTime);
    if (keys[GLFW_KEY_S])
        camera.processMovement(CameraMovement::BACKWARD, deltaTime);
    if (keys[GLFW_KEY_A])
        camera.processMovement(CameraMovement::LEFT, deltaTime);
    if (keys[GLFW_KEY_D])
        camera.processMovement(CameraMovement::RIGHT, deltaTime);
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);

    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
            keys[key] = true;
        else if (action == GLFW_RELEASE)
            keys[key] = false;
    }

    if (keys[GLFW_KEY_LEFT_SHIFT])
        isSprint = true;
    else if (!keys[GLFW_KEY_LEFT_SHIFT])
        isSprint = false;
}

void mouseCallback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        mouseLastX = xpos;
        mouseLastY = ypos;

        firstMouse = false;
    }

    GLfloat xoffset = xpos - mouseLastX;
    GLfloat yoffset = mouseLastY - ypos;

    mouseLastX = xpos;
    mouseLastY = ypos;

    camera.processMouseMovement(xoffset, yoffset);
}

int main()
{
    const valve::BSP map("maps/cube.bsp");

    std::cout << "[src_renderer]: Init window" << std::endl;

    if (!glfwInit())
        std::cout << "[GLFW Init]: Can't init GLFW! Check library including" << std::endl;

    glfwSetErrorCallback(glfwError);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

    GLFWwindow* mainWindow = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "src_renderer", nullptr, nullptr);

    if (mainWindow == nullptr)
    {
        std::cout << "[GLFW Error]: Failed to create window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(mainWindow);

    glfwSetKeyCallback(mainWindow, keyCallback);
    glfwSetCursorPosCallback(mainWindow, mouseCallback);
    glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK)
    {
        std::cout << "[GLEW Error]: Failed to init GLEW" << std::endl;
        return -1;
    }

    int width, height;

    Shader shaderColorBox("shaders/vertex.vert", "shaders/color.frag");

    GLuint VAO, VBO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // hl2 map
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * map.verticesToDraw.size(), map.verticesToDraw.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), static_cast<GLvoid*>(0));

    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glEnable(GL_DEPTH_TEST);

    while (!glfwWindowShouldClose(mainWindow))
    {
        glfwGetFramebufferSize(mainWindow, &width, &height);
        glViewport(0, 0, width, height);

        const GLfloat currentFrame = static_cast<GLfloat>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glm::mat4x4 matView(1.0f);
        matView = camera.getViewMatrix();

        // Model, view, projection matrix init
        glm::mat4x4 matModel(1.0f);
        glm::mat4x4 matProjection(1.0f);

        float aspect = static_cast<GLfloat>(width) / static_cast<GLfloat>(height);
        matProjection = glm::perspective(glm::radians(50.0f), aspect, 0.1f, 100.f);

        glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shaderColorBox.use();

        // Draw map
        glBindVertexArray(VAO);

        glm::mat4 _matModel(1.0f);
        _matModel = glm::translate(_matModel, glm::vec3(0.0f, 0.0f, 0.0f));

        GLuint _matModelLocation = glGetUniformLocation(shaderColorBox.program, "matModel");
        GLuint _matViewLocation = glGetUniformLocation(shaderColorBox.program, "matView");
        GLuint _matProjLocation = glGetUniformLocation(shaderColorBox.program, "matProj");

        glUniformMatrix4fv(_matModelLocation, 1, GL_FALSE, glm::value_ptr(_matModel));
        glUniformMatrix4fv(_matViewLocation, 1, GL_FALSE, glm::value_ptr(matView));
        glUniformMatrix4fv(_matProjLocation, 1, GL_FALSE, glm::value_ptr(matProjection));

        glDrawArrays(GL_TRIANGLES, 0, map.verticesToDraw.size() / 3);

        glfwPollEvents();
        moveCamera();

        glfwSwapBuffers(mainWindow);
    }

    std::cout << "[src_renderer]: Closing window..." << std::endl;

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glfwTerminate();

    return 0;
}
