#ifndef TEAPOT_H
#define TEAPOT_H

#include "Drawable.h"
#include <GL/freeglut.h>

class Teapot : public Drawable {
    public:
        Teapot(Material* material);
        void draw() override;
        void drawShadows(float shadowColor[4]) override;
};

#endif
