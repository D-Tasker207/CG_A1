#ifndef DRAWABLE_H
#define DRAWABLE_H

#include <unordered_map>
#include <string>
#include "Material.h"
#include "Texture.h"

using namespace std;

class Drawable {
    public:
        Drawable() {}
        /*
        * MUST ENABLE LIGHTING BEFORE FUNCTION CALL
        * MUST ENABLE TEXTURE BEFORE FUNCTION CALL IF TEXTURE IS USED
        */
        virtual void draw() = 0;
        /*
        * Draw the object with a shadow color
        * MUST DISABLE LIGHTING BEFORE FUNCTION CALL
        */
        virtual void drawShadows(float shadowColor[4]) = 0;

        void addMaterial(std::string materialName, Material* material) { this->materials[materialName] = material; };
        void addTexture(std::string textureName, std::string imgPath) { this->textures[textureName] = new Texture(imgPath); };
        Texture* getTexture(std::string textureName) { return this->textures[textureName]; };
        Material* getMaterial(std::string materialName) { return this->materials[materialName]; };

        private:
            std::unordered_map<std::string, Material*> materials;
            std::unordered_map<std::string, Texture*> textures;
};

#endif