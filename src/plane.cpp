#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#endif

#include "Plane.h"
#include <GL/freeglut.h>

Plane::Plane(Material* material, float components[4], float xRange[2], float zRange[2]) {
    addMaterial("default", material);
    std::copy(xRange, xRange + 2, this->xRange);
    std::copy(zRange, zRange + 2, this->zRange);

    A = components[0];
    B = components[1];
    C = components[2];
    D = components[3];

    uScale = 1.0;
    vScale = 1.0;
}

void Plane::setTextureScaling(float uScale, float vScale) {
    this->uScale = uScale;
    this->vScale = vScale;
}

void Plane::setComponents(float components[4]) {
    A = components[0];
    B = components[1];
    C = components[2];
    D = components[3];
}

void Plane::draw() {
    getMaterial("default")->apply();

    float totalXRange = xRange[1] - xRange[0];
    float totalZRange = zRange[1] - zRange[0];

    glBegin(GL_QUADS);
        glNormal3f(A, B, C);
        for(int i = xRange[0]; i < xRange[1]; i++){
            for(int j = zRange[0]; j < zRange[1]; j++){
                glTexCoord2f(uScale*(i+xRange[0])/totalXRange, vScale*(j+zRange[0])/totalZRange);
                glVertex3f(i, calculateY(i, j), j);
                
                glTexCoord2f(uScale*(i+xRange[0])/totalXRange, vScale*((j+1)+zRange[0])/totalZRange);
                glVertex3f(i, calculateY(i, j+1), j+1);

                glTexCoord2f(uScale*((i+1)+zRange[0])/totalXRange, vScale*((j+1)+zRange[0])/totalZRange);
                glVertex3f(i+1, calculateY(i+1, j+1), j+1);

                glTexCoord2f(uScale*((i+1)+zRange[0])/totalXRange, vScale*(j+zRange[0])/totalZRange);
                glVertex3f(i+1, calculateY(i+1, j), j);
            }
        }
    glEnd();
}

void Plane::drawShadows(float shadowColor[4]) {
    glColor4fv(shadowColor);
    glBegin(GL_QUADS);
        glNormal3f(A, B, C);
        for(int i = xRange[0]; i < xRange[1]; i++){
            for(int j = zRange[0]; j < zRange[1]; j++){
                glVertex3f(i, calculateY(i, j), j);
                glVertex3f(i+1, calculateY(i+1, j), j);
                glVertex3f(i+1, calculateY(i+1, j+1), j+1);
                glVertex3f(i, calculateY(i, j+1), j+1);
            }
        }
    glEnd();
}