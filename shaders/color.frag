#version 330 core

in vec3 Normal;
in vec3 FragPos;

out vec4 color;

uniform vec3 cameraPos;
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 objColor;

void main()
{
    float ambientCoef = 0.1f;
    float specularCoef = 0.5f;

    vec3 viewDir = normalize(cameraPos - FragPos);

    vec3 ambient = ambientCoef * lightColor;

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);

    vec3 reflectDir = reflect(-lightDir, norm);
    float specComponent = pow(max(dot(viewDir, reflectDir), 0.0f), 32);

    vec3 specular = specularCoef * specComponent * lightColor;
    float diff = max(dot(norm, lightDir), 0.0f);
    vec3 diffuse = diff * lightColor;

    vec3 result = (ambient + diffuse + specular) * objColor;

    color = vec4(result, 1.0f);
}
