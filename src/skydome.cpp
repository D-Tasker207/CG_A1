#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#endif

#include "SkyDome.h"
#include <GL/freeglut.h>

SkyDome::SkyDome() : quadric(gluNewQuadric()){
    addTexture("sky", new Texture("Skydome.hdr"));
    gluQuadricDrawStyle(quadric, GLU_FILL);
    gluQuadricNormals(quadric, GLU_SMOOTH);
    glEnable(GL_TEXTURE_2D);
    gluQuadricTexture(quadric, GL_TRUE);
}

SkyDome::~SkyDome() {
    gluDeleteQuadric(quadric);
}

// ENABLE TEXTURE BEFOREHAND
void SkyDome::draw() {
    getTexture("sky")->bind(0);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glRotatef(90, 1.0, 0.0, 0.0);
    gluSphere(quadric, 100, 100, 100);
}

void SkyDome::drawShadows(float shadowColor[4]) {
    return;
}