#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <math.h>

#include "shader.h"

static void glfwError(int id, const char* desc)
{
    std::cout << "[GLFW Error]: " << desc << std::endl;
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

int main()
{
    glfwSetErrorCallback(glfwError);
    glfwInit();
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

    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK)
    {
        std::cout << "[GLEW Error]: Failed to init GLEW" << std::endl;
        return -1;
    }

    int width, height;

    Shader shader1("vertex.glsl", "fragment.glsl");
    Shader shader2("vertex.glsl", "fragment2.glsl");

    GLfloat vertices1[] = {
            -0.8, -0.5, 0.0, 1.0, 0.0, 0.0,
            -0.5, 0.3, 0.0, 0.0, 1.0, 0.0,
            -0.2, -0.5, 0.0, 0.0, 0.0, 1.0
    };

    GLfloat vertices2[] = {
            0.2, -0.5, 0.0,
            0.5, 0.5, 0.0,
            0.8, -0.5, 0.0
    };

    GLuint vao1;
    glGenVertexArrays(1, &vao1);
    glBindVertexArray(vao1);

    GLuint vbo1;
    glGenBuffers(1, &vbo1);
    glBindBuffer(GL_ARRAY_BUFFER, vbo1);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), static_cast<GLvoid*>(0));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), reinterpret_cast<GLvoid*>(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    GLuint vao2;
    glGenVertexArrays(1, &vao2);
    glBindVertexArray(vao2);

    GLuint vbo2;
    glGenBuffers(1, &vbo2);
    glBindBuffer(GL_ARRAY_BUFFER, vbo2);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), static_cast<GLvoid*>(0));
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

    while(!glfwWindowShouldClose(mainWindow))
    {
        glfwGetFramebufferSize(mainWindow, &width, &height);
        glViewport(0, 0, width, height);

        glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader1.use();

        GLint posOffsetLocation = glGetUniformLocation(shader1.program, "posOffset");
        glUniform1f(posOffsetLocation, 0.5f);

        glBindVertexArray(vao1);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);

        shader2.use();

        GLfloat timeValue = glfwGetTime();
        GLfloat redV = std::abs((sin(timeValue * 2)));
        GLint vertexColorLocation = glGetUniformLocation(shader2.program, "animColor");
        glUniform3f(vertexColorLocation, redV, 0.0f, 0.0f);

        glBindVertexArray(vao2);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);

        glfwPollEvents();
        glfwSwapBuffers(mainWindow);
    }

    std::cout << "[src_renderer]: Closing window..." << std::endl;

    glfwTerminate();

    return 0;
}
