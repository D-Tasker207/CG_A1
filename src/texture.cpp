#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#endif

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#endif

#include "Texture.h"
#include "stb_image.h"
#include <filesystem>
#include <iostream>
#include <GL/freeglut.h>

static std::string getBasePath(const std::string& filePath);

Texture::Texture(std::string imgName){
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    std::string basePath = getBasePath(__FILE__);
    std::string imgPath = basePath + TEXTURE_PATH + imgName;

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
        stbi_image_free(imgData);
    } else {
        std::cout << "Failed to load texture" << std::endl;
    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

Texture::~Texture(){
    glDeleteTextures(1, &textureID);
}

void Texture::bind(unsigned int unit){
    // glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, textureID);
}

static std::string getBasePath(const std::string& filePath) {
        // This function extracts the base path (directory) from the file path
        size_t found = filePath.find_last_of("/\\");
        return filePath.substr(0, found);
}