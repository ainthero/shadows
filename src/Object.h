#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <assimp/scene.h>
#include "Shader.h"

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
};

class Mesh {
private:
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    GLuint VAO = 0;
    GLuint VBO = 0;
    GLuint EBO = 0;

public:
    Mesh(const std::vector<Vertex> &vertices, const std::vector<GLuint> &indices);

    void render();
};

class Object {
private:
    std::vector<Mesh> meshes;

    void load(const char *path);

    void processNode(aiNode *node, const aiScene *scene);

    static Mesh processMesh(aiMesh *mesh);

public:
    explicit Object(const char *path);

    void render(Shader& shader);
};
