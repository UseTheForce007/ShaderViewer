#pragma once
#include <string>
#include <glad/gl.h>

class ObjModel {
public:
    ObjModel(const std::string& path);
    ~ObjModel();
    void draw() const;
private:
    GLuint VAO = 0, VBO = 0;
    GLsizei vertexCount = 0;
};
