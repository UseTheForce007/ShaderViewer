#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "shader.h"
#include "model.h"
#include "camera.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main() {
    // glfw: initialize and configure
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // glfw window creation
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Shader Viewer", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // if (!gladLoaderLoadGL((GLADuserptrloadfunc)glfwGetProcAddress, NULL)) {
    if (!gladLoaderLoadGL()) {    
    std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    glEnable(GL_DEPTH_TEST);

    // build and compile shaders
    Shader shader("shaders/default.vert", "shaders/default.frag");

    // load models
    Model model("assets/suzanne.obj");

    // render loop
    while (!glfwWindowShouldClose(window)) {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        processInput(window);
        
        // check for shader changes
        if (shader.checkForChanges()) {
            shader.reload();
        }

        // render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.use();

        // camera/view transformation
        glm::mat4 view = camera.getViewMatrix();
        glm::mat4 projection = camera.getProjectionMatrix();
        glm::mat4 modelMat = glm::mat4(1.0f);
        
        shader.setMat4("projection", projection);
        shader.setMat4("view", view);
        shader.setMat4("model", modelMat);
        shader.setFloat("time", currentFrame);

        model.draw();

        // Print camera position
        std::cout << "Camera position: (" 
                  << camera.getPosition().x << ", "
                  << camera.getPosition().y << ", "
                  << camera.getPosition().z << ")\n";

        // Print view matrix
        std::cout << "View matrix:\n";
        const float* viewPtr = (const float*)glm::value_ptr(view);
        for (int i = 0; i < 4; ++i) {
            std::cout << viewPtr[i*4+0] << " " << viewPtr[i*4+1] << " " << viewPtr[i*4+2] << " " << viewPtr[i*4+3] << "\n";
        }

        // Print projection matrix
        std::cout << "Projection matrix:\n";
        const float* projPtr = (const float*)glm::value_ptr(projection);
        for (int i = 0; i < 4; ++i) {
            std::cout << projPtr[i*4+0] << " " << projPtr[i*4+1] << " " << projPtr[i*4+2] << " " << projPtr[i*4+3] << "\n";
        }

        GLenum err;
        while ((err = glGetError()) != GL_NO_ERROR) {
            std::cout << "OpenGL error: " << err << std::endl;
        }

        // glfw: swap buffers and poll IO events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    camera.processMouse(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    camera.processScroll(static_cast<float>(yoffset));
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    camera.processKeyboard(deltaTime);
}
