#include "TestCamera.h"

TestCamera::TestCamera() : position(1.0f, 0.0f, 5.0f) {}

glm::mat4 TestCamera::getViewMatrix() const {
    return glm::lookAt(position, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
}

glm::mat4 TestCamera::getProjectionMatrix(float aspect) const {
    return glm::perspective(glm::radians(45.0f), aspect, 0.1f, 100.0f);
}
