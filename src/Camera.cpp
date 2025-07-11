#include "Camera.h"

Camera::Camera() : position(1.0f, 0.0f, 5.0f) {}

glm::mat4 Camera::getViewMatrix() const {
    return glm::lookAt(position, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
}

glm::mat4 Camera::getProjectionMatrix(float aspect) const {
    return glm::perspective(glm::radians(zoom), aspect, 0.1f, 100.0f);
}

void Camera::processScroll(float yoffset) {
    zoom -= yoffset;
    if (zoom < 1.0f)
        zoom = 1.0f;
    if (zoom > 45.0f)
        zoom = 45.0f;
}

void Camera::startDrag(double xpos, double ypos) {
    dragging = true;
    lastMousePos = glm::dvec2(xpos, ypos);
}

void Camera::updateDrag(double xpos, double ypos) {
    if (!dragging) return;
    glm::dvec2 currentPos(xpos, ypos);
    glm::dvec2 delta = currentPos - lastMousePos;

    // Simple pan: move camera position in X/Y plane (feel free to adjust sensitivity)
    float sensitivity = 0.01f;
    position.x -= static_cast<float>(delta.x) * sensitivity;
    position.y += static_cast<float>(delta.y) * sensitivity;

    lastMousePos = currentPos;
}

void Camera::endDrag() {
    dragging = false;
}
