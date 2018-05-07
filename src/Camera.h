#pragma  once
#ifndef Camera_h
#define Camera_h

#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/quaternion.hpp"

class Camera {
public:
    Camera() {}
    ~Camera() {}
    
    void update();
    glm::mat4 getViewMatrix();
    void setRotation(glm::vec3 rot);
    void setRotation(glm::quat rot);
    void lookAt(glm::vec3 pos); // Look at pos, maintaining current up vector
    void lookAt(glm::vec3 pos, glm::vec3 up); // Look at pos with specified up vector
    
    glm::vec3 pos = glm::vec3(0);
    glm::vec3 rot = glm::vec3(0);
    
    glm::vec3 vel = glm::vec3(0); // Position velocity per frame (local to camera frame)
    glm::vec3 rotVel = glm::vec3(0); // Rotation velocity per frame (local to camera frame)
    
private:
    void setRotation(glm::mat4 rot); // Doesn't update rot
    
    glm::vec3 lookVector = glm::vec3(0, 0, -1);
    glm::vec3 up = glm::vec3(0, 1, 0);
};

#endif /* Camera_h */
