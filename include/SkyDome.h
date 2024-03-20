#ifndef SKYDOME_H
#define SKYDOME_H

#include "Drawable.h"
#include <GL/freeglut.h>

class SkyDome : public Drawable {
    public:
        SkyDome();
        ~SkyDome();
        void draw() override;
        void drawShadows(float shadowColor[4]) override;
    private:
        GLUquadric* quadric;
};

#endif