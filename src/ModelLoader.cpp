#include "ModelLoader.h"

Model::~Model() {
    
}

bool Model::load(
    std::string path){

    std::ifstream inFileStream;
    inFileStream.open(path, std::ifstream::in);
    char c = inFileStream.get();

    while (inFileStream.good()) {
        std::cout << c;
        c = inFileStream.get();
    }

    inFileStream.close();
    return true;
}