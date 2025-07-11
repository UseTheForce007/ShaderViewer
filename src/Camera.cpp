#include "Camera.h"

Camera::Camera() : position(0.0f, 0.0f, 5.0f) {}

/*      Copilot wants to calculate position this way but I don't think it matters
 *      this calculation is inconsequential
 */     
// Camera::Camera() : radius(5.0f), yaw(0.0f), pitch(0.0f) {
//     float yawRad = glm::radians(yaw);
//     float pitchRad = glm::radians(pitch);
//     position.x = radius * cosf(pitchRad) * sinf(yawRad);
//     position.y = radius * sinf(pitchRad);
//     position.z = radius * cosf(pitchRad) * cosf(yawRad);

glm::mat4 Camera::getViewMatrix() const {
    return glm::lookAt(position, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
}

glm::mat4 Camera::getProjectionMatrix(float aspect) const {
    return glm::perspective(glm::radians(zoom), aspect, 0.1f, 100.0f);
}

void Camera::processScroll(float yoffset) {
    if (useFovZoom) {
        zoom -= yoffset;
        if (zoom < 1.0f) zoom = 1.0f;
        if (zoom > 90.0f) zoom = 90.0f;
    } else {
        radius -= 0.1 * yoffset;
        if (radius < 1.0f) radius = 1.0f;
        if (radius > 50.0f) radius = 50.0f;

        // Update position after zoom
        float yawRad = glm::radians(yaw);   // yaw is 0
        float pitchRad = glm::radians(pitch); // pitch is 0
        position.x = radius * cosf(pitchRad) * sinf(yawRad); // = 5 * 1 * 0 = 0
        position.y = radius * sinf(pitchRad);                // = 5 * 0 = 0
        position.z = radius * cosf(pitchRad) * cosf(yawRad); // = 5 * 1 * 1 = 5
    }
}

void Camera::startDrag(double xpos, double ypos) {
    dragging = true;
    lastMousePos = glm::dvec2(xpos, ypos);
}

void Camera::updateDrag(double xpos, double ypos) {
    if (!dragging) return;
    glm::dvec2 currentPos(xpos, ypos);
    glm::dvec2 delta = currentPos - lastMousePos;

    float sensitivity = 0.3f; // Adjust for feel
    yaw   -= static_cast<float>(delta.x) * sensitivity;
    pitch += static_cast<float>(delta.y) * sensitivity; // Invert Y here

    // Clamp pitch to avoid flipping
    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;

    // Update camera position based on spherical coordinates
    float yawRad = glm::radians(yaw);
    float pitchRad = glm::radians(pitch);
    position.x = radius * cosf(pitchRad) * sinf(yawRad);
    position.y = radius * sinf(pitchRad);
    position.z = radius * cosf(pitchRad) * cosf(yawRad);

    lastMousePos = currentPos;
}

void Camera::endDrag() {
    dragging = false;
}
