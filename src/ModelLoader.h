#ifndef MODELLOADER_H
#define MODELLOADER_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include <glm/glm.hpp>
class Model {
    public:
        Model();
        ~Model();
        bool load(
            std::string path);
    private:
        std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
        std::vector<glm::vec3>& _vertices;
        std::vector<glm::vec2>& _uvs;
        std::vector<glm::vec3>& _normals;
};
#endif