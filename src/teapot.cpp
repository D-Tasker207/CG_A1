#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#endif

#include <GL/freeglut.h>
#include "Teapot.h"

void Teapot::drawObject() {
    glutSolidTeapot(1);
}