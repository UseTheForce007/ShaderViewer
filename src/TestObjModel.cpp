#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>
#include "TestObjModel.h"
#include <vector>
#include <iostream>

TestObjModel::TestObjModel(const std::string& path) {
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;
    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, path.c_str());
    if (!warn.empty()) std::cout << "TinyObjLoader warning: " << warn << std::endl;
    if (!err.empty()) std::cerr << "TinyObjLoader error: " << err << std::endl;
    if (!ret) {
        std::cerr << "Failed to load OBJ: " << path << std::endl;
        return;
    }
    std::vector<float> vertices;
    for (const auto& shape : shapes) {
        for (const auto& idx : shape.mesh.indices) {
            vertices.push_back(attrib.vertices[3 * idx.vertex_index + 0]);
            vertices.push_back(attrib.vertices[3 * idx.vertex_index + 1]);
            vertices.push_back(attrib.vertices[3 * idx.vertex_index + 2]);
        }
    }
    vertexCount = static_cast<GLsizei>(vertices.size() / 3);
    std::cout << "Loaded OBJ vertex count: " << vertexCount << std::endl;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

TestObjModel::~TestObjModel() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void TestObjModel::draw() const {
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, vertexCount);
    glBindVertexArray(0);
}
