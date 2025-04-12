#include "camera.h"
#include <iostream>

Camera::Camera(glm::vec3 position) :
    front(glm::vec3(0.0f, 0.0f, -1.0f)),
    movementSpeed(5.0f),
    mouseSensitivity(0.1f),
    zoom(45.0f),
    worldUp(glm::vec3(0.0f, 1.0f, 0.0f)),
    yaw(-90.0f),
    pitch(0.0f),
    firstMouse(true),
    lastX(0.0f),
    lastY(0.0f) {
    this->position = position;
    updateCameraVectors();
}

void Camera::processKeyboard(MovementDirection direction, float deltaTime) {
    float velocity = movementSpeed * deltaTime;

    if (direction == FORWARD)
        position += front * velocity;
    if (direction == BACKWARD)
        position -= front * velocity;
    if (direction == LEFT)
        position -= right * velocity;
    if (direction == RIGHT)
        position += right * velocity;
    if (direction == UP)
        position += worldUp * velocity;
    if (direction == DOWN)
        position -= worldUp * velocity;
}

void Camera::processMouseMovement(float xoffset, float yoffset, bool constrainPitch) {
    if (firstMouse) {
        lastX = xoffset;
        lastY = yoffset;
        firstMouse = false;
    }

    float xpos = xoffset;
    float ypos = yoffset;

    xoffset = xpos - lastX;
    yoffset = lastY - ypos; // reversed since y-coordinates range from bottom to top

    lastX = xpos;
    lastY = ypos;

    xoffset *= mouseSensitivity;
    yoffset *= mouseSensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (constrainPitch) {
        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;
    }

    updateCameraVectors();
}

glm::mat4 Camera::getViewMatrix() const {
    return glm::lookAt(position, position + front, up);
}

void Camera::updateCameraVectors() {
    glm::vec3 newFront;
    newFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    newFront.y = sin(glm::radians(pitch));
    newFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(newFront);

    right = glm::normalize(glm::cross(front, worldUp));
    up = glm::normalize(glm::cross(right, front));
}