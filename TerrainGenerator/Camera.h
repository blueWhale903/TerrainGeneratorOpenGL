#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
public:
    enum MovementDirection {
        FORWARD,
        BACKWARD,
        LEFT,
        RIGHT,
        UP,
        DOWN
    };

    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f));

    void processKeyboard(MovementDirection direction, float deltaTime);
    void processMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);

    glm::mat4 getViewMatrix() const;
	glm::vec3 getPosition() const { return position; }
    float getZoom() const { return zoom; }

private:
    void updateCameraVectors();

    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;

    float yaw;
    float pitch;

    float movementSpeed;
    float mouseSensitivity;
    float zoom;

    bool firstMouse;
    float lastX;
    float lastY;
};
