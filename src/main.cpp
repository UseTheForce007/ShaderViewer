/*
*What is the following:
*   GLAD? 
*       Creates the window and OpenGL context. Think: “Where do I draw?”
*   GLFW?
*       Loads OpenGL functions into your app. Think: “How do I call OpenGL?”
*   glm?
*       OpenGL Mathematics Library
*   OpenGL?
*       Handles GPU state, shaders, vertex buffers, etc. Think: “Do the actual drawing.”
*   
*
*    
*
*
*
*
*    
*
*
*
*
*    
*
*
*
*
*/

// OpenGL loader (GLAD) and context/window management (GLFW)
#include <glad/gl.h>
#include <GLFW/glfw3.h>

// Standard libraries for I/O and math
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

// Project headers
#include "Shader.h"     // Handles GLSL shader program compilation & usage
#include "ObjModel.h"   // Loads and draws a 3D .obj model
#include "Camera.h"     // Provides view and projection matrices

// Callback to adjust OpenGL viewport when the window is resized
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

// Poll keyboard input (ESC key to close window)
void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

int main() {
    // Step 1: Initialize GLFW (window + context management)
    if (!glfwInit()) {
        std::cout << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // Step 2: Create a windowed mode window and OpenGL context
    GLFWwindow* window = glfwCreateWindow(800, 600, "ShaderViewer Test", NULL, NULL);
    if (!window) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Step 3: Make the window's OpenGL context current
    glfwMakeContextCurrent(window);

    // Set a callback for window resize events
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


    // Step 4: Load OpenGL function pointers via GLAD
    // Must be called AFTER you have a valid OpenGL context (i.e., after glfwMakeContextCurrent).
    if (!gladLoaderLoadGL()) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }

    // Log the active OpenGL version
    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;

    // Step 5: Load and set up core objects
    // Local for now
    Shader shader("shaders/default.vert", "shaders/default.frag"); // Loads and compiles shaders
    ObjModel model("assets/suzanne.obj");                          // Loads a 3D model from .obj file
    Camera camera;                                                 // Camera providing view/projection matrices

    // Step 6: Main rendering loop
    while (!glfwWindowShouldClose(window)) {
        processInput(window);  // Check for user input (ESC to exit)

        // Clear the screen with a dark gray color
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Use shader program and set matrices
        shader.use();

        // Identity model matrix (no transformations yet)
        glm::mat4 modelMat = glm::mat4(1.0f);

        // Get view matrix from camera (defines camera position/direction)
        glm::mat4 view = camera.getViewMatrix();

        // Calculate projection matrix based on window aspect ratio
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        glm::mat4 projection = camera.getProjectionMatrix(width / (float)height);

        // Send matrices to the shader
        shader.setMat4("model", glm::value_ptr(modelMat));
        shader.setMat4("view", glm::value_ptr(view));
        shader.setMat4("projection", glm::value_ptr(projection));

        // Draw the 3D model
        model.draw();

        // Swap front and back buffers (double-buffered rendering)
        glfwSwapBuffers(window);

        // Poll for window events (input, resize, etc.)
        glfwPollEvents();
    }

    // Cleanup and exit
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
