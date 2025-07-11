#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
public:
    Camera();
    glm::mat4 getViewMatrix() const;
    glm::mat4 getProjectionMatrix(float aspect) const;
    glm::vec3 position;
    float zoom = 45.0f;
    bool dragging = false;
    glm::dvec2 lastMousePos{0.0, 0.0};

    //Camera movement functions
    void processScroll(float yoffset);
    void startDrag(double xpos, double ypos);
    void updateDrag(double xpos, double ypos);
    void endDrag();
};
