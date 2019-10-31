#ifndef TEXTURE_H
#define TEXTURE_H

#include <glew.h>
#include <string>

class Texture {
    public:
        Texture(const char* filepath);
        ~Texture();
        GLuint getTextureID();
        GLint width, height;
    private: 
    GLuint id;
};
#endif