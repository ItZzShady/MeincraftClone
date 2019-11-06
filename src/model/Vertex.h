#pragma once
#ifndef VERTEX_H
#define VERTEXH

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Vertex {
    public:
    Vertex(glm::vec3 position);
    Vertex(float x, float y, float z);
    ~Vertex();  

    glm::vec3 getPosition();
    void setPosition(glm::vec3 position);
    private:
    glm::vec3 _position;
};
#endif