#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

uniform mat4 matModel;
uniform mat4 matView;
uniform mat4 matProj;

out vec3 Normal;
out vec3 FragPos;

void main()
{
    gl_Position = matProj * matView * matModel * vec4(position, 1.0);
    FragPos = vec3(matModel * vec4(position, 1.0f));
    Normal = mat3(transpose(inverse(matModel))) * normal;
}
