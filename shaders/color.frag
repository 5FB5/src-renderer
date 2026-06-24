#version 330 core

in vec3 Normal;
in vec3 FragPos;
in vec3 LightPos;

out vec4 color;

uniform vec3 cameraPos;
uniform vec3 lightColor;
uniform vec3 objColor;

void main()
{
    float ambientCoef = 0.1f;

    vec3 ambient = ambientCoef * lightColor;
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(cameraPos.xyz - FragPos);

    float diff = max(dot(norm, viewDir), 0.0f);
    vec3 diffuse = diff * lightColor;

    vec3 result = (ambient + diffuse) * objColor;

    color = vec4(result, 1.0f);
}
