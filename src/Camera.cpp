#include <stdio.h>
#include "Camera.h"
#include "glm/gtc/matrix_transform.hpp"

glm::quat createQuat(glm::vec3 lookVector, glm::vec3 up) {
    glm::mat4 m;
    m[0] = glm::vec4(glm::cross(up, -lookVector), 0);
    m[1] = glm::vec4(glm::cross(-lookVector, glm::vec3(m[0][0], m[0][1], m[0][2])), 0);
    m[2] = glm::vec4(-lookVector, 0);
    m[3] = glm::vec4(0, 0, 0, 1);
    return glm::quat_cast(m);
}

void Camera::update() {
    // Update rotation
    this->rot += rotVel;
    setRotation(this->rot);
    
    // Update position
    glm::vec3 axisX = glm::normalize(glm::cross(up, -lookVector));
    glm::vec3 delta = glm::vec3(0);
    delta += lookVector * vel.z;
    delta += axisX * -vel.x;
    pos += delta;
}

glm::mat4 Camera::getViewMatrix() {
    // Create camera rotation matrix
    glm::vec3 axisX = glm::normalize(glm::cross(up, -lookVector));
    glm::vec3 axisY = glm::normalize(glm::cross(-lookVector, axisX));
    glm::vec3 axisZ = glm::normalize(-lookVector);
    glm::mat4 rotMat;
    rotMat[0] = glm::vec4(axisX, 0);
    rotMat[1] = glm::vec4(axisY, 0);
    rotMat[2] = glm::vec4(axisZ, 0);
    rotMat[3] = glm::vec4(0, 0, 0, 1);
    rotMat = glm::transpose(rotMat);
    
    // Translate the camera, preserving rotation
    glm::mat4 V = glm::translate(rotMat, pos);
    return V;
}

void Camera::setRotation(glm::vec3 rot) {
    this->rot = rot;
    glm::mat4 rotMatX = glm::rotate(glm::mat4(1), rot.x, glm::vec3(1, 0, 0));
    glm::mat4 rotMatY = glm::rotate(glm::mat4(1), rot.y, glm::vec3(0, 1, 0));
    glm::mat4 rotMatZ = glm::rotate(glm::mat4(1), rot.z, glm::vec3(0, 0, 1));
    glm::mat4 rotMat = rotMatY * rotMatX * rotMatZ;
    setRotation(rotMat);
}

void Camera::setRotation(glm::quat rot) {
    this->rot = glm::eulerAngles(rot);
    glm::mat4 rotMat = glm::mat4_cast(rot);
    setRotation(rotMat);
}

void Camera::setRotation(glm::mat4 rot) {
    lookVector = glm::normalize(glm::vec3(-rot[2]));
    up = glm::normalize(glm::vec3(rot[1]));
}

void Camera::lookAt(glm::vec3 pos) {
    lookVector = glm::normalize(pos + this->pos); // Camera pos is negated
    setRotation(createQuat(lookVector, up));
}

void Camera::lookAt(glm::vec3 pos, glm::vec3 up) {
    lookVector = glm::normalize(pos + this->pos); // Camera pos is negated
    this->up = up;
    setRotation(createQuat(lookVector, up));
}
