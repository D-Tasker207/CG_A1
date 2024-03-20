#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>

class Texture {
    public:
        Texture(std::string imgPath);
        void bind(unsigned int uint);
    private:
        unsigned int textureID;
};

#endif