#include <fstream>
#include <sstream>
#include "Shader.h"

Shader::Shader(const std::string &vert_path, const std::string &frag_path) {
    GLuint vert_shader = compileShader(vert_path, GL_VERTEX_SHADER);
    GLuint frag_shader = compileShader(frag_path, GL_FRAGMENT_SHADER);
    id = glCreateProgram();
    glAttachShader(id, vert_shader);
    glAttachShader(id, frag_shader);
    glLinkProgram(id);
    GLint success;
    GLchar info_log[512];
    glGetProgramiv(id, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(id, 512, nullptr, info_log);
        std::string msg = std::string("Program linking failed: ") + info_log;
        throw std::runtime_error(msg);
    }
    glDeleteShader(vert_shader);
    glDeleteShader(frag_shader);
}

std::string Shader::readFileToString(const std::string &path) {
    std::ifstream input_file(path);
    return {(std::istreambuf_iterator<char>(input_file)), std::istreambuf_iterator<char>()};
}

GLuint Shader::compileShader(const std::string &path, GLenum type) {
    std::string code = readFileToString(path);
    GLuint shader = glCreateShader(type);
    const char *c_string = code.c_str();
    glShaderSource(shader, 1, &c_string, nullptr);
    glCompileShader(shader);
    GLint success;
    GLchar info_log[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, nullptr, info_log);
        std::string msg = std::string("Shader compilation failed: ") + path + "\n" + info_log;
        throw std::runtime_error(msg);
    }
    return shader;
}

GLint Shader::getUniformLocation(const std::string &name) const {
    return glGetUniformLocation(id, name.c_str());
}

void Shader::use() {
    glUseProgram(id);
}

void Shader::setBool(const std::string &name, GLboolean value) const {
    glProgramUniform1i(id, getUniformLocation(name), (int) value);
}

void Shader::setInt(const std::string &name, GLint value) const {
    glProgramUniform1i(id, getUniformLocation(name), value);
}

void Shader::setFloat(const std::string &name, GLfloat value) const {
    glProgramUniform1f(id, getUniformLocation(name), value);
}

void Shader::setMat4(const std::string &name, const glm::mat4 &value) const {
    glProgramUniformMatrix4fv(id, getUniformLocation(name), 1, GL_FALSE, &value[0][0]);
}

void Shader::setVec3(const std::string &name, const glm::vec3 &value) const {
    glProgramUniform3fv(id, getUniformLocation(name), 1, &value.r);
}
