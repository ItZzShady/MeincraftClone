#include "Mesh.h"

#include "Log.h"

Mesh::Mesh(void) : numVertices(0), numIndices(0) {
    for (int buffer = 0; buffer < MAX_BUFFER; buffer++) {
        _bufferObject[buffer] = 0;
    }
}

Mesh::~Mesh() {
    glDeleteVertexArrays(1, &_arrayObject);
    glDeleteBuffers(MAX_BUFFER, _bufferObject);
}

void Mesh::draw() {
    glBindVertexArray(_arrayObject);
    glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Mesh::bufferData() {
    LOG::Print("Try to generate buffer data", INFO);
    if (!_vertices.empty()) {
        LOG::Print("Vertices are not empty", INFO);
        if (!_indices.empty()) {
            LOG::Print("Indices are not empty", INFO);
            glGenVertexArrays(1, &_arrayObject);
            glBindVertexArray(_arrayObject);
            LOG::Print("Vertex array generated", INFO);

            glGenBuffers(1, &_bufferObject[VERTEX_BUFFER]);
            glBindBuffer(GL_ARRAY_BUFFER, _bufferObject[VERTEX_BUFFER]);
            glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(glm::vec3), &_vertices[0], GL_STATIC_DRAW);
            LOG::Print("Vertex buffer generated", INFO);

            glGenBuffers(1, &_bufferObject[INDEX_BUFFER]);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _bufferObject[INDEX_BUFFER]);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(int), &_indices[0], GL_STATIC_DRAW);
            LOG::Print("Element buffer generated", INFO);
            
            glEnableVertexAttribArray(VERTEX_BUFFER);
            glVertexAttribPointer(VERTEX_BUFFER, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

            LOG::Print("Buffer data generated", INFO);

            return;
        }

        LOG::Print("Mesh cannot create buffer data. No indice data", ERROR);

        return;
    }

    LOG::Print("Mesh cannot create buffer data. No vertex data", ERROR);
}

std::vector<Vertex> Mesh::getVertices() {
    return _vertices;
}
void Mesh::setVertices(std::vector<Vertex> vertices) {
    _vertices = vertices;
    numVertices = vertices.size();
}

std::vector<int> Mesh::getIndices() {
    return _indices;
}

void Mesh::setIndices(std::vector<int> indices) {
    _indices = indices;
    numIndices = indices.size();
    bufferData();
}