#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <GL/freeglut.h>

#define TEXTURE_PATH "/../resources/textures/"

class Texture {
    public:
        Texture(std::string imgPath);
        ~Texture();
        void bind(unsigned int uint);
    private:
        unsigned int textureID;
};

#endif