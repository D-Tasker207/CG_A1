#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#endif

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#endif

#include "Texture.h"
#include "stb_image.h"
#include <string>
#include <iostream>
#include <GL/freeglut.h>

Texture::Texture(std::string imgPath){
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, channels;
    unsigned char* imgData = stbi_load(imgPath.c_str(), &width, &height, &channels, 0);
    if (imgData){
        switch(channels){
            case 1:
                glTexImage2D(GL_TEXTURE_2D, 0, channels, width, height, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, imgData);
                break;
            case 3:
                glTexImage2D(GL_TEXTURE_2D, 0, channels, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imgData);
                break;
            case 4:
                glTexImage2D(GL_TEXTURE_2D, 0, channels, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imgData);
                break;  
        }
    } else {
        std::cout << "Failed to load texture" << std::endl;
    }
}

void Texture::bind(unsigned int unit){
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, textureID);
}