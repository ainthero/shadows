#pragma once

#include <string>
#include <vector>
#include "common.h"

class Shader {
private:
    static std::string readFileToString(const std::string &path);

    static GLuint compileShader(const std::string &code, GLenum type);

    GLint getUniformLocation(const std::string &name) const;

public:
    GLuint id;

    Shader(const std::string &vert_path, const std::string &frag_path);

    void use();

    void setBool(const std::string &name, GLboolean value) const;

    void setInt(const std::string &name, GLint value) const;

    void setFloat(const std::string &name, GLfloat value) const;

    void setVec3(const std::string &name, const glm::vec3 &value) const;

    void setMat4(const std::string &name, const glm::mat4 &value) const;
};
