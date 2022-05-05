#pragma once

#include "common.h"
#include "Shader.h"

class Light {
private:
    glm::vec3 position;
    glm::vec3 color;
    glm::vec3 target;
    GLuint depth_map = 0;
    glm::mat4 model;
    glm::mat4 light_space;

public:
    bool is_enabled;

    Light(const glm::vec3 &position, const glm::vec3 &color, const glm::vec3 &target = {0, 0, 0});

    void genDepthMap(GLuint resolution);

    void genLightSpace(const glm::mat4 &projection);

    const glm::vec3 &getPosition() const;

    const glm::vec3 &getColor() const;

    GLuint getDepthMap() const;

    const glm::mat4 &getModel() const;

    const glm::mat4 &getLightSpace() const;

    glm::mat4 getLightView() const;
};
