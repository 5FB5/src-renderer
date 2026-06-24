#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

uniform mat4 matModel;
uniform mat4 matView;
uniform mat4 matProj;

out vec3 Normal;
out vec3 FragPos;
out vec3 LightPos;

uniform vec3 lightPos;

void main()
{
    gl_Position = matProj * matView * matModel * vec4(position, 1.0);
    FragPos = vec3(matView * matModel * vec4(position, 1.0f));
    Normal = mat3(transpose(inverse(matView * matModel))) * normal;
    LightPos = vec3(matView * vec4(lightPos, 1.0f));
}
