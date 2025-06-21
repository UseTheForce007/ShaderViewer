#pragma once

#include <string>
#include <filesystem>
#include <glad/gl.h>
#include <glm/glm.hpp>

class Shader {
public:
    Shader(const std::string& vertexPath, const std::string& fragmentPath);
    ~Shader();

    void use();
    void reload();
    bool checkForChanges();

    void setMat4(const std::string& name, const glm::mat4& matrix);
    void setVec3(const std::string& name, const glm::vec3& value);
    void setFloat(const std::string& name, float value);

private:
    GLuint loadFromFiles(const std::string& vertexPath, const std::string& fragmentPath);
    GLuint compileShader(const std::string& source, GLenum type);

    GLuint m_programId;
    std::string m_vertexPath;
    std::string m_fragmentPath;
    std::filesystem::file_time_type m_lastVertexWrite;
    std::filesystem::file_time_type m_lastFragmentWrite;
};
