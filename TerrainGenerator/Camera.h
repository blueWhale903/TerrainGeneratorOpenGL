#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
public:
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    float yaw;
    float pitch;
    float speed;
    float sensitivity;

    Camera(glm::vec3 pos = glm::vec3(0.0f, 10.0f, 10.0f))          : position(pos), worldUp(glm::vec3(0.0f, 1.0f, 0.0f)),
        yaw(-90.0f), pitch(-30.0f), speed(2.5f), sensitivity(0.1f) {
        updateVectors();
    }

    glm::mat4 getViewMatrix() {
        return glm::lookAt(position, position + front, up);
    }

    void processKeyboard(int direction, float deltaTime) {
        float velocity = speed * deltaTime;
        if (direction == 0) position += front * velocity;         if (direction == 1) position -= front * velocity;         if (direction == 2) position -= glm::normalize(glm::cross(front, up)) * velocity;         if (direction == 3) position += glm::normalize(glm::cross(front, up)) * velocity;     }

    void processMouse(float xoffset, float yoffset) {
        xoffset *= sensitivity;
        yoffset *= sensitivity;

        yaw += xoffset;
        pitch += yoffset;

        if (pitch > 89.0f) pitch = 89.0f;
        if (pitch < -89.0f) pitch = -89.0f;

        updateVectors();
    }

private:
    glm::vec3 worldUp;  
    void updateVectors() {
                        glm::vec3 newFront;
        newFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        newFront.y = sin(glm::radians(pitch));
        newFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        front = glm::normalize(newFront);
        
                glm::vec3 right = glm::normalize(glm::cross(front, worldUp));
        up = glm::normalize(glm::cross(right, front));
    }
};
