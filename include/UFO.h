#ifndef UFO_H
#define UFO_H

#include "Drawable.h"
#include <GL/freeglut.h>

class UFO : public Drawable {
    public:
        UFO();
        void draw() override;
        void drawShadows(float shadowColor[4]) override;
        void setDishAngle(float angle) { dishAngle = angle; };
        void incDishAngle();
        void takeOff();

    private:
        GLUquadric* q;
        float dishAngle;

        void drawBody();
        void drawCanopy();
        void drawLegs();
        void drawAntenna();
        void drawDish();
        void drawAntennaBase();
        void drawLights();
};

#endif