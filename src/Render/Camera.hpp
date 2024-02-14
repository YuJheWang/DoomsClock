#pragma once

#include <SDL2/SDL.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/quaternion.hpp>

class Camera
{
public:

    Camera();

    Camera(const glm::vec3& pos, const glm::vec3& lookAtPos);

    void moveTo(const glm::vec3& pos);

    void moveFront(float step);

    void moveRight(float step);

    void moveUp(float step);

    void lookAt(const glm::vec3& lookAtPos);

    void rotateCamera(const glm::vec3& rotate);  

    glm::mat4 getViewMatrix() const;

private:

    glm::vec3 _pos = glm::vec3(0.0f, 0.0f, 3.0f);

    glm::vec3 _frontVector = glm::vec3(0.0f, 0.0f, -1.0f);

    glm::vec3 _upVector = glm::vec3(0.0f, 1.0f, 0.0f);

    glm::vec3 _rightVector;

    float pitch = 0, yaw = 0, roll = 0;

};

Camera::Camera()
{
    _rightVector = glm::normalize(glm::cross(_frontVector, _upVector));
}

Camera::Camera(const glm::vec3& pos, const glm::vec3& lookAtPos)
{
    moveTo(pos); lookAt(lookAtPos);
}

void Camera::moveTo(const glm::vec3& pos)
{
    _pos = pos;
}

void Camera::moveFront(float step)
{
    _pos += glm::normalize(_frontVector) * step;
}

void Camera::moveRight(float step)
{
    _pos += glm::normalize(_rightVector) * step;
}

void Camera::moveUp(float step)
{
    _pos += glm::normalize(_upVector) * step;
}

void Camera::lookAt(const glm::vec3& lookAtPos)
{
    _frontVector = glm::normalize(lookAtPos - _pos);
}

void Camera::rotateCamera(const glm::vec3& rotate)
{
    pitch += rotate.x, yaw += rotate.y, roll += rotate.z;
    pitch = pitch > 89.0f ?  89.0f : pitch;
    pitch = pitch < -89.0f ? -89.0f : pitch;

    _frontVector = glm::vec3(
        sin(yaw) * cos(pitch),
        sin(pitch),
        -cos(yaw) * cos(pitch)
    );
    _rightVector = glm::normalize(glm::cross(_frontVector, _upVector));
}

glm::mat4 Camera::getViewMatrix() const
{
    return glm::lookAt(
        _pos, _pos + _frontVector, _upVector
    );
}
