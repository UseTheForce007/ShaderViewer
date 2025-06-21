#pragma once
#include <glad/gl.h>

class TestModel {
public:
    TestModel();
    ~TestModel();
    void draw() const;
private:
    GLuint VAO, VBO;
};
