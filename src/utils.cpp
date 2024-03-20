#include "utils.h"
#include <string>
#include <memory>
#include <cmath>

void getShadowMat(float lightPos[4], float planeCoef[4], float shadowMat[16]);

void getShadowMat(float lightPos[4], float planeCoef[4], float shadowMat[16]) {
    float dot = planeCoef[0] * lightPos[0] +
            planeCoef[1] * lightPos[1] +
            planeCoef[2] * lightPos[2] +
            planeCoef[3] * lightPos[3];

    shadowMat[0] = dot - lightPos[0] * planeCoef[0];
    shadowMat[4] = -lightPos[0] * planeCoef[1];
    shadowMat[8] = -lightPos[0] * planeCoef[2];
    shadowMat[12] = -lightPos[0] * planeCoef[3];

    shadowMat[1] = -lightPos[1] * planeCoef[0];
    shadowMat[5] = dot - lightPos[1] * planeCoef[1];
    shadowMat[9] = -lightPos[1] * planeCoef[2];
    shadowMat[13] = -lightPos[1] * planeCoef[3];

    shadowMat[2] = -lightPos[2] * planeCoef[0];
    shadowMat[6] = -lightPos[2] * planeCoef[1];
    shadowMat[10] = dot - lightPos[2] * planeCoef[2];
    shadowMat[14] = -lightPos[2] * planeCoef[3];

    shadowMat[3] = -lightPos[3] * planeCoef[0];
    shadowMat[7] = -lightPos[3] * planeCoef[1];
    shadowMat[11] = -lightPos[3] * planeCoef[2];
    shadowMat[15] = dot - lightPos[3] * planeCoef[3];
}
