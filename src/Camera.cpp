#include "Camera.h"
#include <iostream>

Camera::Camera() :
    speed(0.001f),
    position(glm::vec3(4.0f, 0.0f, 10.0f))  { 

    glm::mat4 view = glm::lookAt(
        glm::vec3(4.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f)
    );
}

Camera::~Camera() {

}

void Camera::update(GLFWwindow* window, GLfloat dt) {
    glfwGetCursorPos(window, &xpos, &ypos);

    horizontalAngle = 3.14f,
    verticalAngle = 0.0f;
    horizontalAngle += speed * float(1024/2 - xpos);
    verticalAngle   += speed * float(768/2 - ypos);

    glm::vec3 direction (
        cos(verticalAngle) * sin(horizontalAngle),
        sin(verticalAngle),
        cos(verticalAngle) * cos(horizontalAngle)
    );

    // Right
     glm::vec3 right = glm::vec3(
        sin(horizontalAngle - 3.14/2.0f),
        0,
        cos(horizontalAngle - 3.14/2.0f)
    );

    // Up
    glm::vec3 up = glm::cross(right, direction);

    // Move forward
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
        position += direction * dt * 5.0f;
    }
    // Move backward
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
        position -= direction * dt * 5.0f;
    }
    // Strafe right
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
        position -= right * dt * 5.0f;
    }
    // Strafe left
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
        position += right * dt * 5.0f;
    }
    // Close window when hit escape
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window, true);
    }

    view = glm::lookAt(
        position,
        position+direction,
        up
    );
}

glm::mat4 Camera::getView() {
    return view;
}

void Camera::setCursorPos(GLdouble x, GLdouble y) {
    xpos = x;
    ypos = y;
}