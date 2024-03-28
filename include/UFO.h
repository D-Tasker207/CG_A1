#ifndef UFO_H
#define UFO_H

#include "Particle.h"
#include "Drawable.h"
#include <GL/freeglut.h>

class UFO : public Drawable {
    public:
        UFO();
        void draw() override;
        void drawShadows(float shadowColor[4]) override;
        void updateSmoke();
        void setDishAngle(float angle) { dishAngle = angle; };
        void incDishAngle();
        void setTakingOff(bool takingOff) { this->takingOff = takingOff; };
    private:
        GLUquadric* q;
        Particle* engineSmoke;
        float takeOffHeight;
        float dishAngle;
        bool takingOff;

        void drawBody();
        void drawCanopy();
        void drawLegs();
        void drawAntenna();
        void drawDish();
        void drawAntennaBase();
        void drawLights();
        void drawSmoke();
};

#endif