#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 800

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <SOIL/SOIL.h>
#include <iostream>
#include <math.h>

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
       camera.movementSpeed = 30.0f;
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
    if(firstMouse)
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
    if (!glfwInit())
        std::cout << "[GLFW Init]: Can't init GLFW! Check library including" << std::endl;

    glfwSetErrorCallback(glfwError);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

    GLFWwindow *mainWindow = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "src_renderer", nullptr, nullptr);

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
    Shader shaderLightBox("shaders/vertexNoNormals.vert", "shaders/light.frag");

    float vertices[] = {
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
            0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
            0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
            0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
            0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
            0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
            0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

            0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
            0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
            0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
            0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
            0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
            0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
            0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
            0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
            0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
            0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
            0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
            0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };

    GLuint VAOs[3], VBOs[3];

    glGenVertexArrays(3, VAOs);
    glGenBuffers(3, VBOs);

    // light
    glBindVertexArray(VAOs[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), static_cast<GLvoid*>(0));

    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // box
    glBindVertexArray(VAOs[1]);

    glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), static_cast<GLvoid*>(0));
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), reinterpret_cast<GLvoid*>(3 * sizeof(GLfloat)));

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glEnable(GL_DEPTH_TEST);

    glm::vec3 lightPosition(2.0f, 1.5f, -3.0f);

    float rotationRadius = 2.f;

    while(!glfwWindowShouldClose(mainWindow))
    {
        glfwGetFramebufferSize(mainWindow, &width, &height);
        glViewport(0, 0, width, height);

        GLfloat currentFrame = static_cast<GLfloat>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glm::mat4x4 matView(1.0f);
        matView = camera.getViewMatrix();

        // Model, view, projection matrix init
        glm::mat4x4 matModel(1.0f);
        glm::mat4x4 matProjection(1.0f);

        float aspect = static_cast<GLfloat>(width) / static_cast<GLfloat>(height);
        matProjection = glm::perspective(glm::radians(50.0f), aspect, 0.1f, 100.f);

        GLfloat lightPosX = std::sin(glfwGetTime()) * rotationRadius;
        GLfloat lightPosZ = std::cos(glfwGetTime()) * rotationRadius;

        glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Draw light
        glBindVertexArray(VAOs[0]);

        shaderLightBox.use();

        GLuint _matModelLocation = glGetUniformLocation(shaderLightBox.program, "matModel");
        GLuint _matViewLocation = glGetUniformLocation(shaderLightBox.program, "matView");
        GLuint _matProjLocation = glGetUniformLocation(shaderLightBox.program, "matProj");

        GLuint _lightColorLocation = glGetUniformLocation(shaderLightBox.program, "lightColor");
        GLuint _objColorLocation = glGetUniformLocation(shaderLightBox.program, "objColor");

        glUniform3f(_lightColorLocation, 1.0f, 1.0f, 1.0f);
        glUniform3f(_objColorLocation, 1.0f, 0.5f, 0.31f);

        glm::mat4x4 matBox(1.0f);
        matBox = glm::translate(matBox, glm::vec3(lightPosX, lightPosition.y, lightPosZ));

        glUniformMatrix4fv(_matModelLocation, 1, GL_FALSE, glm::value_ptr(matBox));
        glUniformMatrix4fv(_matViewLocation, 1, GL_FALSE, glm::value_ptr(matView));
        glUniformMatrix4fv(_matProjLocation, 1, GL_FALSE, glm::value_ptr(matProjection));

        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Draw color box
        glBindVertexArray(VAOs[1]);

        shaderColorBox.use();

        GLuint cameraPosLocation = glGetUniformLocation(shaderColorBox.program, "cameraPos");
        GLuint lightPosLocation = glGetUniformLocation(shaderColorBox.program, "lightPos");

        GLuint matModelLocation = glGetUniformLocation(shaderColorBox.program, "matModel");
        GLuint matViewLocation = glGetUniformLocation(shaderColorBox.program, "matView");
        GLuint matProjLocation = glGetUniformLocation(shaderColorBox.program, "matProj");

        GLuint lightColorLocation = glGetUniformLocation(shaderColorBox.program, "lightColor");
        GLuint objColorLocation = glGetUniformLocation(shaderColorBox.program, "objColor");

        glUniform3f(cameraPosLocation, camera.position.x, camera.position.y, camera.position.z);
        glUniform3f(lightPosLocation, lightPosX, lightPosition.y, lightPosZ);
        glUniform3f(lightColorLocation, 1.0f, 1.0f, 1.0f);
        glUniform3f(objColorLocation, 0.5f, 0.5f, 0.31f);

        matModel = glm::scale(matModel, glm::vec3(2.0f, 2.0f, 2.0f));

        glUniformMatrix4fv(matModelLocation, 1, GL_FALSE, glm::value_ptr(matModel));
        glUniformMatrix4fv(matViewLocation, 1, GL_FALSE, glm::value_ptr(matView));
        glUniformMatrix4fv(matProjLocation, 1, GL_FALSE, glm::value_ptr(matProjection));

        glDrawArrays(GL_TRIANGLES, 0, 36);

        glfwPollEvents();
        moveCamera();

        glfwSwapBuffers(mainWindow);
    }

    std::cout << "[src_renderer]: Closing window..." << std::endl;

    glDeleteVertexArrays(3, VAOs);
    glDeleteBuffers(3, VBOs);
    glfwTerminate();

    return 0;
}
