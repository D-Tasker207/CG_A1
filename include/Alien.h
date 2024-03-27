#ifndef ALIEN_H
#define ALIEN_H

#include "Drawable.h"

class Alien : public Drawable{
    public:
        Alien();
        void draw() override;
        void drawShadows(float shadowColor[4]) override;

        void setArmsPose(
            float rightArmAngle, float leftArmAngle,
            float rightForeArmAngle, float leftForeArmAngle,
            float *rightArmAxis[3], float *leftArmAxis[3]
        );
        void setIdleHeight(int keyframe);
        void setWalkingFrame(int keyframe);
        void setIsIdle(bool isIdle);
        bool getIsIdle() { return isIdle; }
    private:
        void drawFeet();
        void drawEyes();
        void drawMouth();
        void drawLegs();
        void drawArms();
        void drawBody();

        GLUquadric *q;
        bool isIdle;
        float idleHeight;
        float walkingAngle;
        float rightArmAngle;
        float leftArmAngle;
        float rightForeArmAngle;
        float leftForeArmAngle;
        float rightArmAxis[3];
        float leftArmAxis[3];
};
#endif