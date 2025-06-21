#include "shader.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>

Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath)
    : m_vertexPath(vertexPath), m_fragmentPath(fragmentPath) {
    m_programId = loadFromFiles(vertexPath, fragmentPath);
    m_lastVertexWrite = std::filesystem::last_write_time(vertexPath);
    m_lastFragmentWrite = std::filesystem::last_write_time(fragmentPath);
}

Shader::~Shader() {
    glDeleteProgram(m_programId);
}

void Shader::use() {
    glUseProgram(m_programId);
}

void Shader::reload() {
    GLuint newProgram = loadFromFiles(m_vertexPath, m_fragmentPath);
    if (newProgram) {
        glDeleteProgram(m_programId);
        m_programId = newProgram;
        m_lastVertexWrite = std::filesystem::last_write_time(m_vertexPath);
        m_lastFragmentWrite = std::filesystem::last_write_time(m_fragmentPath);
        std::cout << "Shader reloaded successfully\n";
    }
}

bool Shader::checkForChanges() {
    try {
        auto vertexTime = std::filesystem::last_write_time(m_vertexPath);
        auto fragmentTime = std::filesystem::last_write_time(m_fragmentPath);
        
        if (vertexTime != m_lastVertexWrite || fragmentTime != m_lastFragmentWrite) {
            return true;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error checking shader files: " << e.what() << std::endl;
    }
    return false;
}

GLuint Shader::loadFromFiles(const std::string& vertexPath, const std::string& fragmentPath) {
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;

    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;

        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();

        vShaderFile.close();
        fShaderFile.close();

        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    } catch (std::ifstream::failure& e) {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
        return 0;
    }

    GLuint vertex = compileShader(vertexCode, GL_VERTEX_SHADER);
    GLuint fragment = compileShader(fragmentCode, GL_FRAGMENT_SHADER);
    
    if (!vertex || !fragment) return 0;

    GLuint program = glCreateProgram();
    glAttachShader(program, vertex);
    glAttachShader(program, fragment);
    glLinkProgram(program);

    // Check for linking errors
    GLint success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[1024];
        glGetProgramInfoLog(program, 1024, nullptr, infoLog);
        std::cerr << "ERROR::PROGRAM_LINKING_ERROR\n" << infoLog << std::endl;
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);

    return program;
}

GLuint Shader::compileShader(const std::string& source, GLenum type) {
    GLuint shader = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);

    // Check for compilation errors
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[1024];
        glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
        std::cerr << "ERROR::SHADER_COMPILATION_ERROR of type: "
                  << (type == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT") << "\n"
                  << infoLog << std::endl;
    }

    return shader;
}

void Shader::setMat4(const std::string& name, const glm::mat4& matrix) {
    glUniformMatrix4fv(glGetUniformLocation(m_programId, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::setVec3(const std::string& name, const glm::vec3& value) {
    glUniform3fv(glGetUniformLocation(m_programId, name.c_str()), 1, glm::value_ptr(value));
}

void Shader::setFloat(const std::string& name, float value) {
    glUniform1f(glGetUniformLocation(m_programId, name.c_str()), value);
}
