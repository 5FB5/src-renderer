//
// Created by valera on 16.09.23.
//

#include "camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 up, GLfloat yaw, GLfloat pitch ) : front(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(SPEED), mouseSensivity(SENSIVITY)
{
    this->position = position;
    this->worldUp = up;
    this->yaw = yaw;
    this->pitch = pitch;

    updateCameraVectors();
}

void Camera::updateCameraVectors()
{
    glm::vec3 _front;

    _front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    _front.y = sin(glm::radians(pitch));
    _front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

    front = glm::normalize(_front);
    right = glm::normalize(glm::cross(front, worldUp));
    up = glm::normalize(glm::cross(right, front));
}

glm::mat4x4 Camera::getViewMatrix()
{
    return glm::lookAt(position, position + front, up);
}

void Camera::processMovement(CameraMovement direction, GLfloat deltaTime)
{
    GLfloat velocity = movementSpeed * deltaTime;

    if (direction == CameraMovement::FORWARD)
        position += front * velocity;
    if (direction == CameraMovement::BACKWARD)
        position -= front * velocity;
    if (direction == CameraMovement::LEFT)
        position -= right * velocity;
    if (direction == CameraMovement::RIGHT)
        position += right * velocity;
}

void Camera::processMouseMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch)
{
    xoffset *= mouseSensivity;
    yoffset *= mouseSensivity;

    yaw += xoffset;
    pitch += yoffset;

    if (constrainPitch)
    {
        if (pitch > 89.f)
            pitch = 89.f;
        else if (pitch < -89.f)
            pitch = -89.f;
    }

    updateCameraVectors();
};
