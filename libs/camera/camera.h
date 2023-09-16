//
// Created by valera on 16.09.23.
//

#ifndef SRC_RENDERER_CAMERA_H
#define SRC_RENDERER_CAMERA_H

#include <iostream>

#include <math.h>
#include <vector>
#include <GL/glew.h>

#include "glm.hpp"
#include "gtc/matrix_transform.hpp"

enum CameraMovement: int
{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

const GLfloat PITCH = 0.0f;
const GLfloat YAW = -90.0f;
const GLfloat SPEED = 3.0f;
const GLfloat SENSIVITY = 0.05f;

class Camera
{

private:
    void updateCameraVectors();

public:
    explicit Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), GLfloat yaw = YAW, GLfloat pitch = PITCH);

    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;

    GLfloat yaw;
    GLfloat pitch;

    GLfloat movementSpeed = 0.0f;
    GLfloat mouseSensivity = 0.0f;

    void processMovement(CameraMovement direction, GLfloat deltaTime);
    void processMouseMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch = true);

    glm::mat4x4 getViewMatrix();
};


#endif //SRC_RENDERER_CAMERA_H
