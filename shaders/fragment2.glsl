#version 330 core

uniform vec3 animColor;

out vec4 color;

void main()
{
    color = vec4(animColor, 1.0f);
}
