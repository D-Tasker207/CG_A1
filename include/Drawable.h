#ifndef DRAWABLE_H
#define DRAWABLE_H

#include "Material.h"
#include <vector>

class Drawable {
    public:
        Drawable(Material* material) : material(material) {}
        void draw();
        void setMaterial(Material* material);
        void addTexture(char* imgPath);
        void drawShadows(float shadowColor[4]);
        void applyMaterial();
        void getTexture(unsigned int unit);
        Material getMaterial() { return *material; };
    protected:
        virtual void drawObject() = 0;        
    private:
        Material* material;
        std::vector<Texture*> textures;
};

#endif