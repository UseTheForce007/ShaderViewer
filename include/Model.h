#pragma once
#include <glad/gl.h>

class Model {
public:
    Model();
    ~Model();
    void draw() const;
private:
    GLuint VAO, VBO;
};
