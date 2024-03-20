#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#endif

#include <GL/freeglut.h>
#include "Teapot.h"

void Teapot::draw() {
    getMaterial("default")->apply();
    glutSolidTeapot(1);
}

void Teapot::drawShadows(float shadowColor[4]) {
    glColor4fv(shadowColor);
    glutSolidTeapot(1);
}