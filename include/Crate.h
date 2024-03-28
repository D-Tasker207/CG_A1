#ifndef CRATE_H
#define CRATE_H

#include "Drawable.h"

class Crate : public Drawable{
    public:
        Crate();
        void draw() override;
        void drawShadows(float shadowColor[4]) override;
    private:
        void drawCrate();
};

#endif