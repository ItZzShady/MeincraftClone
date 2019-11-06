#include "Vertex.h"

Vertex::Vertex(glm::vec3 position) {
    setPosition(position);
}

Vertex::Vertex(float x, float y, float z) {
    setPosition(glm::vec3(x, y, z));
}

Vertex::~Vertex() {
}

glm::vec3 Vertex::getPosition() {
    return _position;
}

void Vertex::setPosition(glm::vec3 position) {
    _position = position;
}