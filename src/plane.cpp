#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#endif

#include "Plane.h"
#include "utils.h"
#include <GL/freeglut.h>

Plane::Plane(Material* material, float components[4], float xRange[2], float zRange[2], float lineColor[4]) {
    addMaterial("default", material);
    std::copy(lineColor, lineColor + 4, this->lineColor);
    std::copy(xRange, xRange + 2, this->xRange);
    std::copy(zRange, zRange + 2, this->zRange);
    this->A = components[0];
    this->B = components[1];
    this->C = components[2];
    this->D = components[3];
}

void Plane::setComponents(float components[4]) {
    this->A = components[0];
    this->B = components[1];
    this->C = components[2];
    this->D = components[3];
}

void Plane::draw() {
    getMaterial("default")->apply();
    glColor3fv(lineColor);			//Floor colour

    glDisable(GL_LIGHTING);
        for(int i = this->xRange[0]; i <= this->xRange[1]; i ++)
        {
            glBegin(GL_LINES);			//A set of grid lines on the xz-plane
                glVertex3f(this->xRange[0], 0, i);
                glVertex3f(this->xRange[1], 0, i);
            glEnd();
        }
        for(int i = this->zRange[0]; i <= this->zRange[1]; i ++)
        {
            glBegin(GL_LINES);			//A set of grid lines on the xz-plane
                glVertex3f(i, 0, this->zRange[0]);
                glVertex3f(i, 0, this->zRange[1]);
            glEnd();
        }

    glEnable(GL_LIGHTING);

    glBegin(GL_QUADS);
        for(int i = this->xRange[0]; i < this->xRange[1]; i++){
            for(int j = this->zRange[0]; j < this->zRange[1]; j++){
                glTexCoord2f(i/100.0f, j/100.0f);
                glVertex3f(i, calculateY(i, j), j);
                
                glTexCoord2f(i/100.0f, (j+1)/100.0f);
                glVertex3f(i+1, calculateY(i+1, j), j);
                
                glTexCoord2f((i+1)/100.0f, (j+1)/100.0f);
                glVertex3f(i+1, calculateY(i+1, j+1), j+1);
                
                glTexCoord2f((i+1)/100.0f, j/100.0f);
                glVertex3f(i, calculateY(i, j+1), j+1);
            }
        }
    glEnd();
}

void Plane::drawShadows(float shadowColor[4]) {
    glColor4fv(shadowColor);
    glBegin(GL_QUADS);
        for(int i = this->xRange[0]; i < this->xRange[1]; i++){
            for(int j = this->zRange[0]; j < this->zRange[1]; j++){
                glVertex3f(i, 0, j);
                glVertex3f(i+1, 0, j);
                glVertex3f(i+1, 0, j+1);
                glVertex3f(i, 0, j+1);
            }
        }
    glEnd();
}