//
// Created by valera on 11.09.23.
//

#include "shader.h"

Shader::Shader(const GLchar *vertexShaderPath, const GLchar *fragmentShaderPath)
{
    std::string vertexShaderSrc;
    std::string fragmentShaderSrc;

    std::ifstream vertexShaderFile;
    std::ifstream fragmentShaderFile;

    vertexShaderFile.exceptions(std::fstream::badbit);
    fragmentShaderFile.exceptions(std::fstream::badbit);

    try
    {
        std::stringstream vertexShaderStream, fragmentShaderStream;

        vertexShaderFile.open(vertexShaderPath, std::ios::binary);
        fragmentShaderFile.open(fragmentShaderPath, std::ios::binary);

        vertexShaderStream << vertexShaderFile.rdbuf();
        fragmentShaderStream << fragmentShaderFile.rdbuf();

        vertexShaderFile.close();
        fragmentShaderFile.close();

        vertexShaderSrc = vertexShaderStream.str();
        fragmentShaderSrc = fragmentShaderStream.str();
    }
    catch (std::ifstream::failure exception)
    {
        std::cout << "[Shader] Shader file reading error: " << exception.what() << std::endl;
    }

    const GLchar *vertexShaderCode = vertexShaderSrc.c_str();
    const GLchar *fragmentShaderCode = fragmentShaderSrc.c_str();

    GLuint vertexShader, fragmentShader;
    GLint success;
    GLchar infoLog[512];

    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderCode, nullptr);
    glCompileShader(vertexShader);

    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
        std::cout << "[Shader]: Vertex shader compile error: " << infoLog << std::endl;
    }

    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderCode, nullptr);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
        std::cout << "[Shader]: Fragment shader compile error: " << infoLog << std::endl;
    }

    program = glCreateProgram();

    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    glGetProgramiv(program, GL_LINK_STATUS, &success);

    if (!success)
    {
        glGetProgramInfoLog(program, 512, nullptr, infoLog);
        std::cout << "[Shader]: Program link error: " << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void Shader::use()
{
    glUseProgram(program);
}