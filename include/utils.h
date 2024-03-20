#ifndef UTILS_H
#define UTILS_H

#include <cmath>
#include <string>

#define toRadians(angle) (angle * M_PI / 180)
#define toDegrees(angle) (angle * 180 / M_PI)

extern void transpose(float** matrix, float** transposed);
extern void getShadowMat(float lightPos[4], float planeCoef[4], float** shadowMat);

#endif