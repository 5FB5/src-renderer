#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <sstream>
#include <fstream>
#include <iostream>

static void glfwError(int id, const char* desc)
{
    std::cout << "[GLFW Error]: " << desc << std::endl;
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

std::string getShaderSourceFromFile(std::string filename)
{
    std::stringstream buffer;
    std::ifstream file(filename, std::ios::binary);

    buffer << file.rdbuf();

    file.close();

    return buffer.str();
}

int main()
{
    glfwSetErrorCallback(glfwError);
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

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

    glfwGetFramebufferSize(mainWindow, &width, &height);

    glViewport(0, 0, width, height);

    GLchar infoLog[512];
    GLint success;

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

    std::string vertShaderStr = getShaderSourceFromFile("vertex.glsl");
    const GLchar *vertShaderSrc = vertShaderStr.c_str();

    glShaderSource(vertexShader, 1, &vertShaderSrc, nullptr);
    glCompileShader(vertexShader);

    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if (success == 0)
    {
        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
        std::cout << "[Vert shader compilation]: Error: " << infoLog << std::endl;
    }

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    std::string fragShaderStr = getShaderSourceFromFile("fragment.glsl");
    const GLchar *fragmentShaderSrc = fragShaderStr.c_str();

    glShaderSource(fragmentShader, 1, &fragmentShaderSrc, nullptr);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
        std::cout << "[Frag shader compilation]: Error: " << infoLog << std::endl;
    }

    GLuint shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram,vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        std::cout << "[Shader Program Link] Error: " << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    GLfloat vertices[] =
            {
                    -0.5f, -0.5f, 0.0f,
                    0.5f, -0.5f, 0.0f,
                    0.0f, 0.5f, 0.0f
            };

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), static_cast<GLvoid*>(0));
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

    while(!glfwWindowShouldClose(mainWindow))
    {
        glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);

        glfwPollEvents();
        glfwSwapBuffers(mainWindow);
    }

    std::cout << "[src_renderer]: Closing window..." << std::endl;

    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glfwTerminate();

    return 0;
}
