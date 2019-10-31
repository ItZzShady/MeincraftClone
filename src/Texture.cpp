#include "Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

Texture::Texture(const char* filepath) {
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);

    stbi_set_flip_vertically_on_load(true);
    unsigned char* textureSource = stbi_load(filepath, &width, &height, nullptr, STBI_rgb);
    if (textureSource == 0) {

    }
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, textureSource);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    stbi_image_free(textureSource);
}

Texture::~Texture() {
    glDeleteTextures(1, &id);
}

GLuint Texture::getTextureID() {
    return id;
}