#ifndef UFO_H
#define UFO_H

#include "Drawable.h"
#include <GL/freeglut.h>

class UFO : public Drawable {
    public:
        UFO();
        void draw() override;
        void drawShadows(float shadowColor[4]) override;
    private:
        void drawBody();
        void drawCanopy();
        void drawLegs();
        void drawAntenna();
        void drawLights();

        int bodyRevolutionVertices = 18;
        float bodyRevolutionLineX[18] = { 0, 5, 2.5, 2.5, 5, 10, 15, 20, 25, 20, 20, 25, 20, 15, 10, 7.5, 7.5, 0 };
        float bodyRevolutionLineY[18] = { 0, 0, 0.25, 0.5, 0.75, 1, 1.25, 1.75, 2.5, 2.5, 3.5, 3.5, 4.25, 4.75, 5, 5, 4, 4 };
};

#endif