#ifndef FACTORY_H
#define FACTORY_H

#include <GL/freeglut.h>
#include "Drawable.h"

class Factory : public Drawable{
    public:
        Factory(int lightID);
        void draw() override;
        void drawShadows(float shadowColor[4]) override;
        void updateSpotlightAngle(int keyframe);
    private:
        void drawSmokeStack();
        void drawShed();
        void drawSearchLight();

        GLUquadric *q;
        GLuint lightID;
        float spotlightAngle = 30.0;
        float spotlightPos[3] = {0, 0, 0};
        float spotlightDir[3] = {0, -1, 0};
        


};

#endif