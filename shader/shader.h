//
// Created by valera on 11.09.23.
//

#ifndef SRC_RENDERER_SHADER_H
#define SRC_RENDERER_SHADER_H

#pragma once

#include "GL/glew.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
    explicit Shader(const GLchar *vertexShaderPath, const GLchar *fragmentShaderPath);

    GLuint program;

    void use();
};

#endif //SRC_RENDERER_SHADER_H
