#ifndef DRAWABLE_H
#define DRAWABLE_H

#include "Material.h"

class Drawable {
    public:
        Drawable(Material* material) : material(material) {}
        void draw();
        void setMaterial(Material* material);
        void drawShadows(float shadowColor[4]);

    protected:
        virtual void drawObject() = 0;

        Material* material;
};

#endif