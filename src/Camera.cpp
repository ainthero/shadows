#include "Camera.h"
#include "glm/gtc/matrix_transform.hpp"

void Camera::translate(Camera::Direction direction) {
    switch (direction) {
        case Direction::FORWARD:
            position += direction_vec * translation_sens;
            break;
        case Direction::BACKWARD:
            position -= direction_vec * translation_sens;
            break;
        case Direction::RIGHT:
            position += right_vec * translation_sens;
            break;
        case Direction::LEFT:
            position -= right_vec * translation_sens;
            break;
    }
}

void Camera::updateVecs() {
    direction_vec = glm::normalize(glm::vec3(
            cos(glm::radians(yaw)) * cos(glm::radians(pitch)),
            sin(glm::radians(pitch)),
            sin(glm::radians(yaw)) * cos(glm::radians(pitch))
    ));
    right_vec = glm::normalize(glm::cross(direction_vec, {0, 1, 0}));
    up_vec = glm::normalize(glm::cross(right_vec, direction_vec));
}

glm::mat4 Camera::lookAt() {
    return glm::lookAt(position, position + direction_vec, up_vec);
}

Camera::Camera(const glm::vec3 &position, const glm::vec3 &target) : position(position) {
    direction_vec = glm::normalize(target - position);
    yaw = glm::degrees(atan2f(direction_vec.z, direction_vec.x));
    pitch = glm::degrees(asinf(direction_vec.y));
    updateVecs();
}

void Camera::rotate(int x, int y) {
    yaw -= rotation_sens * (float) x;
    pitch += rotation_sens * (float) y;
    updateVecs();
}
