#version 330 core

in vec2 TexCoord;
in vec3 outColor;
out vec4 color;

uniform sampler2D baseTexture;
uniform sampler2D secondTexture;
uniform float mixValue;

void main()
{
    color = mix(texture(baseTexture, TexCoord), texture(secondTexture, TexCoord), mixValue);
}
