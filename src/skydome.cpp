#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#endif

#include "SkyDome.h"
#include <GL/freeglut.h>

SkyDome::SkyDome() : quadric(gluNewQuadric()){
    addTexture("sky", new Texture("BlueSkySkybox.png"));
    gluQuadricDrawStyle(quadric, GLU_FILL);
    gluQuadricNormals(quadric, GLU_SMOOTH);
    gluQuadricTexture(quadric, GL_TRUE);
}

SkyDome::~SkyDome() {
    gluDeleteQuadric(quadric);
}

// ENABLE TEXTURE BEFOREHAND
void SkyDome::draw() {
    getTexture("sky")->bind(0);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    gluSphere(quadric, 100, 100, 100);
}

void SkyDome::drawShadows(float shadowColor[4]) {
    return;
}