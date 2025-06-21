#pragma once

#include <vector>
#include <string>
#include <glad/gl.h>
#include <glm/glm.hpp>

class Model {
public:
    Model(const std::string& path);
    ~Model();

    void draw();

private:
    struct Vertex {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 texCoords;
    };

    bool loadFromFile(const std::string& path);
    void setupMesh();

    std::vector<Vertex> m_vertices;
    std::vector<unsigned int> m_indices;
    GLuint m_vao;
    GLuint m_vbo;
    GLuint m_ebo;
};
