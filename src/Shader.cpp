#include "Shader.h"
#include <fstream>    
#include <sstream>    
#include <iostream>   

// Constructor: Takes paths to the vertex and fragment shader source files
Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath) :
    m_VertexPath(vertexPath),
    m_FragmentPath(fragmentPath)
{
    compileAndLink(); // Automatically compile and link the shader on creation
}

// Loads, compiles, and links the shader source code from files
bool Shader::compileAndLink(){
    // Open the vertex and fragment shader files
    std::ifstream vFile(m_VertexPath), fFile(m_FragmentPath);

    
    // Read the contents of the files into strings directly
    std::string vCode((std::istreambuf_iterator<char>(vFile)), std::istreambuf_iterator<char>());
    std::string fCode((std::istreambuf_iterator<char>(fFile)), std::istreambuf_iterator<char>());

    // Convert strings to raw character pointers for OpenGL
    const char* vShaderCode = vCode.c_str();
    const char* fShaderCode = fCode.c_str();
    
    // Compile Vertex Shader
    GLuint vertex = glCreateShader(GL_VERTEX_SHADER); // Create a vertex shader object
    glShaderSource(vertex, 1, &vShaderCode, NULL);     // Provide the source code
    glCompileShader(vertex);                          // Compile the shader

    GLint success;
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success); // Check for compile success
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(vertex, 512, NULL, infoLog); // Get compile error log
        std::cerr << "Vertex shader compilation failed:\n" << infoLog << std::endl;
        return false;
    }

    // Compile Fragment Shader
    GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER); // Create a fragment shader object
    glShaderSource(fragment, 1, &fShaderCode, NULL);       // Provide the source code
    glCompileShader(fragment);                            // Compile the shader

    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success); // Check for compile success
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(fragment, 512, NULL, infoLog); // Get compile error log
        std::cerr << "Fragment shader compilation failed:\n" << infoLog << std::endl;
        return false;
    }

    // Link Shaders into a Program
    m_Program = glCreateProgram();               // Create a GPU shader program
    glAttachShader(m_Program, vertex);           // Attach the compiled vertex shader
    glAttachShader(m_Program, fragment);         // Attach the compiled fragment shader
    glLinkProgram(m_Program);                    // Link them together

    glGetProgramiv(m_Program, GL_LINK_STATUS, &success); // Check if linking was successful
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(m_Program, 512, NULL, infoLog); // Get link error log
        std::cerr << "Shader program linking failed:\n" << infoLog << std::endl;
        return false;
    }

    // Delete the individual shader objects after successful linking
    glDeleteShader(vertex);
    glDeleteShader(fragment);

    return true; // Compilation and linking successful
}

// Destructor: Cleans up the shader program from GPU memory
Shader::~Shader() {
    glDeleteProgram(m_Program);
}

// Activates the shader program for rendering
void Shader::use() const {
    glUseProgram(m_Program);
}

// Set a 4x4 matrix uniform in the shader
void Shader::setMat4(const std::string& name, const float* value) const {
    glUniformMatrix4fv(glGetUniformLocation(m_Program, name.c_str()), 1, GL_FALSE, value);
}

// Set a float uniform in the shader
void Shader::setFloat(const std::string& name, float value) const {
    glUniform1f(glGetUniformLocation(m_Program, name.c_str()), value);
}

// Reloads the shader from source files, useful for hot-reloading while editing GLSL
void Shader::reload(){
    glDeleteProgram(m_Program);  // Remove the old program from GPU
    compileAndLink();            // Recompile and relink the new source
}
