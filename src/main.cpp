#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"
#include "ObjModel.h"
#include "Camera.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

int main() {
    // Step 1: Initialize GLFW
    if (!glfwInit()) {
        std::cout << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // Step 2: Create a windowed mode window and its OpenGL context
    GLFWwindow* window = glfwCreateWindow(800, 600, "ShaderViewer Test", NULL, NULL);
    if (!window) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Step 3: Make the window's context current
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Step 4: Load OpenGL functions with GLAD
    if (!gladLoaderLoadGL()) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }
    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;

    // Step 5: Create test shader, model, and camera
    Shader shader("shaders/default.vert", "shaders/default.frag");
    ObjModel model("assets/suzanne.obj");
    Camera camera;

    // Step 6: Main loop
    while (!glfwWindowShouldClose(window)) {
        processInput(window);
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader.use();
        glm::mat4 modelMat = glm::mat4(1.0f);
        glm::mat4 view = camera.getViewMatrix();
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        glm::mat4 projection = camera.getProjectionMatrix(width / (float)height);
        shader.setMat4("model", glm::value_ptr(modelMat));
        shader.setMat4("view", glm::value_ptr(view));
        shader.setMat4("projection", glm::value_ptr(projection));
        model.draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
