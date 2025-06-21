#pragma once
#include <string>
#include <glad/gl.h>

class TestShader {
public:
    GLuint ID;
    TestShader(const std::string& vertexPath, const std::string& fragmentPath);
    ~TestShader();
    void use() const;
    void setMat4(const std::string& name, const float* value) const;
    void setFloat(const std::string& name, float value) const;
};
