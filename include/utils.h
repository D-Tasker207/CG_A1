#ifndef UTILS_H
#define UTILS_H

#include <cmath>

#define toRadians(angle) (angle * M_PI / 180)
#define toDegrees(angle) (angle * 180 / M_PI)

extern void getShadowMat(float lightPos[4], float planeCoef[4], float shadowMat[16]);
extern void computeFPS();
extern void displayFPS();

#endif