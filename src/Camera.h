#pragma once

#include "common.h"

class Camera {
private:
    GLfloat yaw;
    GLfloat pitch;

    glm::vec3 position;

    glm::vec3 up_vec;
    glm::vec3 direction_vec;
    glm::vec3 right_vec;

    GLfloat translation_sens = 1;
    GLfloat rotation_sens = 0.2;

    void updateVecs();

public:

    enum class Direction {
        FORWARD,
        BACKWARD,
        LEFT,
        RIGHT
    };

    Camera(const glm::vec3 &position, const glm::vec3 &target);

    void translate(Camera::Direction direction);

    void rotate(GLint x, GLint y);

    glm::mat4 lookAt();
};
