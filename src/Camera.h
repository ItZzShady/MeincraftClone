#ifndef CAMERA_H
#define CAMERA_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera {
    public:
    Camera();
    ~Camera();
    glm::mat4 getView();
    void update(GLFWwindow* window, GLfloat dt);
    void setCursorPos(GLdouble x, GLdouble y);
    private:
    double xpos, ypos;
    GLfloat speed;
    GLfloat horizontalAngle;
    GLfloat verticalAngle;
    glm::vec3 position;
    glm::mat4 view;
};
#endif