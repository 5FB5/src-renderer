#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 800

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <SOIL/SOIL.h>
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
    if (!glfwInit())
    {
        std::cout << "[GLFW Init]: Can't init GLFW! Check library including" << std::endl;
    }
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

    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK)
    {
        std::cout << "[GLEW Error]: Failed to init GLEW" << std::endl;
        return -1;
    }

    int width, height;

    Shader shader1("vertex.glsl", "fragment.glsl");
    Shader shader2("vertex.glsl", "fragment2.glsl");
    Shader shader3("vertex.glsl", "fragment3.glsl");

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


    GLfloat vertices3[] = {
            0.4f, 0.4f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,// Top Right
            0.4f, -0.4f, 0.0f, 0.f, 1.0f, 0.0f, 1.0f, 0.0f,// Bottom Right
            -0.3f, -0.4f, 0.0f,0.0f, 0.0f, 1.0f, 0.0f, 0.0f,// Bottom Left
            -0.3f, 0.4f, 0.0f, 0.0f, 0.0f ,0.0f, 0.0f, 1.0f// Top Left
    };

    GLuint indices[] = {
            0, 1, 3,
            1, 2, 3
    };

    int texW, texH;
    unsigned char *image = SOIL_load_image("test.jpg", &texW, &texH, 0, SOIL_LOAD_RGB);

    int tex2W, tex2H;
    unsigned char *image2 = SOIL_load_image("test2.jpg", &tex2W,&tex2H, 0, SOIL_LOAD_RGB);

    GLuint VAOs[3], VBOs[3], ibo, texture, texture2;

    glGenVertexArrays(3, VAOs);
    glGenBuffers(3, VBOs);
    glGenBuffers(1, &ibo);
    glGenTextures(1, &texture);
    glGenTextures(1, &texture2);

    // First triangle
    glBindVertexArray(VAOs[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), static_cast<GLvoid*>(0));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), reinterpret_cast<GLvoid*>(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    // Second triangle
    glBindVertexArray(VAOs[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), static_cast<GLvoid*>(0));
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

    // Rectangle
    glBindVertexArray(VAOs[2]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices3), vertices3, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Generate textures
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texW, texH, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // Set texture filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);  // NOTE the GL_NEAREST Here!
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);  // NOTE the GL_NEAREST Here!
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);

    glBindTexture(GL_TEXTURE_2D, texture2);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tex2W, tex2H, 0, GL_RGB, GL_UNSIGNED_BYTE, image2);
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), static_cast<GLvoid*>(0));
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), reinterpret_cast<GLvoid*>(3 * sizeof(GLfloat)));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), reinterpret_cast<GLvoid*>(6 * sizeof(GLfloat)));

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);

    SOIL_free_image_data(image);
    SOIL_free_image_data(image2);

    while(!glfwWindowShouldClose(mainWindow))
    {
        glfwGetFramebufferSize(mainWindow, &width, &height);
        glViewport(0, 0, width, height);

        glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader1.use();

        glBindVertexArray(VAOs[0]);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);

        shader2.use();

        GLfloat timeValue = glfwGetTime();
        GLfloat redV = std::abs((sin(timeValue * 2)));
        GLfloat blueV = std::abs((sin(timeValue * 4)));
        GLint vertexColorLocation = glGetUniformLocation(shader2.program, "animColor");
        glUniform3f(vertexColorLocation, redV, 0.3f, blueV);

        glBindVertexArray(VAOs[1]);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);

        shader3.use();

        GLfloat texMixValue = std::abs((sin(timeValue * 3)));
        GLint mixValueLocation = glGetUniformLocation(shader3.program, "mixValue");
        glUniform1f(mixValueLocation, texMixValue);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glUniform1i(glGetUniformLocation(shader3.program, "baseTexture"), 0);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);
        glUniform1i(glGetUniformLocation(shader3.program, "secondTexture"), 1);

        glBindVertexArray(VAOs[2]);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glBindVertexArray(0);

        glfwPollEvents();
        glfwSwapBuffers(mainWindow);
    }

    std::cout << "[src_renderer]: Closing window..." << std::endl;

    glfwTerminate();

    return 0;
}
