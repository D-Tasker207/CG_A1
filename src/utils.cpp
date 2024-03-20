#include "utils.h"
#include <string>
#include <memory>
#include <cmath>

void transpose(float** matrix, float** transposed);
void getShadowMat(float lightPos[4], float planeCoef[4], float** shadowMat);

void transpose(float** matrix, float** transposed) {
    transposed = new float*[sizeof(matrix[0])];

    for (int i = 0; i < 4; ++i) {
        transposed[i] = new float[sizeof(matrix) / sizeof(matrix[0])];
        for (int j = 0; j < 4; ++j) {
            transposed[i][j] = matrix[j][i];
        }
    }
}

void getShadowMat(float lightPos[4], float planeCoef[4], float** shadowMat) {
    shadowMat = new float*[4];

    float dot = planeCoef[0] * lightPos[0] +
            planeCoef[1] * lightPos[1] +
            planeCoef[2] * lightPos[2] +
            planeCoef[3] * lightPos[3];

    for (int i = 0; i < 4; ++i) {
        shadowMat[i] = new float[4];
        for (int j = 0; j < 4; ++j)
            shadowMat[i][j] = dot - lightPos[j] * planeCoef[i];
    }
}
