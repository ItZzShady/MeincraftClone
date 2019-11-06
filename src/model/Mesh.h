#pragma once
#ifndef MESH_H
#define MESH_H

#include <vector>

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Vertex.h"

enum MeshBuffer {
    VERTEX_BUFFER, INDEX_BUFFER, MAX_BUFFER
};

class Mesh {
    public:
    Mesh(void);
    ~Mesh(void);
    void draw();

    std::vector<Vertex> getVertices();
    void setVertices(std::vector<Vertex> vertices);
    std::vector<int> getIndices();
    void setIndices(std::vector<int> indices);
    private:
    std::vector<Vertex> _vertices;
    std::vector<int> _indices;
    protected:
    int numVertices, numIndices;
    GLuint _arrayObject;
    GLuint _bufferObject[MAX_BUFFER];

    void bufferData();
};
#endif