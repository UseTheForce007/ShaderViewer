#include "camera.h"
#include <windows.h>

Camera::Camera(const glm::vec3& position)
    : m_position(position)
    , m_front(glm::vec3(0.0f, 0.0f, -1.0f))
    , m_worldUp(glm::vec3(0.0f, 1.0f, 0.0f))
    , m_yaw(-90.0f)
    , m_pitch(0.0f)
    , m_fov(45.0f)
    , m_movementSpeed(2.5f)
    , m_mouseSensitivity(0.1f)
{
    updateCameraVectors();
}

void Camera::processKeyboard(float deltaTime) {
    float velocity = m_movementSpeed * deltaTime;
    if (GetAsyncKeyState('W') & 0x8000)
        m_position += m_front * velocity;
    if (GetAsyncKeyState('S') & 0x8000)
        m_position -= m_front * velocity;
    if (GetAsyncKeyState('A') & 0x8000)
        m_position -= m_right * velocity;
    if (GetAsyncKeyState('D') & 0x8000)
        m_position += m_right * velocity;
}

void Camera::processMouse(float xoffset, float yoffset, bool constrainPitch) {
    xoffset *= m_mouseSensitivity;
    yoffset *= m_mouseSensitivity;

    m_yaw += xoffset;
    m_pitch += yoffset;

    if (constrainPitch) {
        if (m_pitch > 89.0f)
            m_pitch = 89.0f;
        if (m_pitch < -89.0f)
            m_pitch = -89.0f;
    }

    updateCameraVectors();
}

void Camera::processScroll(float yoffset) {
    m_fov -= yoffset;
    if (m_fov < 1.0f)
        m_fov = 1.0f;
    if (m_fov > 45.0f)
        m_fov = 45.0f;
}

glm::mat4 Camera::getViewMatrix() const {
    return glm::lookAt(m_position, m_position + m_front, m_up);
}

glm::mat4 Camera::getProjectionMatrix() const {
    return glm::perspective(glm::radians(m_fov), 800.0f / 600.0f, 0.1f, 100.0f);
}

void Camera::updateCameraVectors() {
    glm::vec3 front;
    front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    front.y = sin(glm::radians(m_pitch));
    front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    m_front = glm::normalize(front);
    m_right = glm::normalize(glm::cross(m_front, m_worldUp));
    m_up = glm::normalize(glm::cross(m_right, m_front));
}
