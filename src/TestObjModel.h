#pragma once
#include <string>
#include <glad/gl.h>

class TestObjModel {
public:
    TestObjModel(const std::string& path);
    ~TestObjModel();
    void draw() const;
private:
    GLuint VAO = 0, VBO = 0;
    GLsizei vertexCount = 0;
};
