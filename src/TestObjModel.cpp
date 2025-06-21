#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>
#include "TestObjModel.h"
#include <vector>
#include <iostream>
#include <cfloat> // for FLT_MAX
#include <algorithm> // for std::min/std::max

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

    // Compute bounding box
    float minX = FLT_MAX, maxX = -FLT_MAX;
    float minY = FLT_MAX, maxY = -FLT_MAX;
    float minZ = FLT_MAX, maxZ = -FLT_MAX;

    for (size_t i = 0; i < attrib.vertices.size(); i += 3) {
        float x = attrib.vertices[i + 0];
        float y = attrib.vertices[i + 1];
        float z = attrib.vertices[i + 2];

        minX = std::min(minX, x); maxX = std::max(maxX, x);
        minY = std::min(minY, y); maxY = std::max(maxY, y);
        minZ = std::min(minZ, z); maxZ = std::max(maxZ, z);
    }

    std::cout << "Model bounds:\n";
    std::cout << "  X: [" << minX << ", " << maxX << "]\n";
    std::cout << "  Y: [" << minY << ", " << maxY << "]\n";
    std::cout << "  Z: [" << minZ << ", " << maxZ << "]\n";

    // Compute midpoint for centering
    float midX = (minX + maxX) * 0.5f;
    float midY = (minY + maxY) * 0.5f;
    float midZ = (minZ + maxZ) * 0.5f;

    const float scale = 0.5f; // try 0.1f, 1.0f, etc.

    for (const auto& shape : shapes) {
        for (const auto& idx : shape.mesh.indices) {
            float x = attrib.vertices[3 * idx.vertex_index + 0];
            float y = attrib.vertices[3 * idx.vertex_index + 1];
            float z = attrib.vertices[3 * idx.vertex_index + 2];

            // Center and scale
            vertices.push_back((x - midX) * scale);
            vertices.push_back((y - midY) * scale);
            vertices.push_back((z - midZ) * scale);
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
