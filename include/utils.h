#ifndef UTILS_H
#define UTILS_H

#include <cmath>

#define toRadians(angle) (angle * M_PI / 180)
#define toDegrees(angle) (angle * 180 / M_PI)

typedef enum {
    VERTEX_NORMAL=0,
    FACE_NORMAL
} NormalType;

typedef enum {
    NORMAL,
    REVERSED
} NormalDirection;

extern void getShadowMat(float lightPos[4], float planeCoef[4], float shadowMat[16]);
extern void computeFPS();
extern void displayFPS();

extern void computeVertexNormals(float x[], float y[], float vnx[], float vny[], int n);
extern void drawRevolution(float vx[], float vy[], float vz[], int nPoints, float angStep, NormalDirection normalDirection);

#endif