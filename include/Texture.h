#ifndef TEXTURE_H
#define TEXTURE_H

class Texture {
    public:
        Texture(const char* imgPath);
        void bind(unsigned int uint);
    private:
        unsigned int textureID;
};

#endif