#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#endif

#include "utils.h"
#include <GL/freeglut.h>
#include <string>
#include <memory>
#include <cmath>

void getShadowMat(float lightPos[4], float planeCoef[4], float shadowMat[16]);
void computeFPS();
void displayFPS();

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

// FPS Counter Variables
int frameCount = 0;
float fps = 0;
int currentTime = 0, previousTime = 0;

void computeFPS(){
	frameCount++;
	currentTime = glutGet(GLUT_ELAPSED_TIME);
	int timeInterval = currentTime - previousTime;

	if (timeInterval > 1000) {
		fps = frameCount * 1000.0f / timeInterval;
		previousTime = currentTime;
		frameCount = 0;
	}
}

void displayFPS(){
    computeFPS();
    char fpsText[15];
    snprintf(fpsText, sizeof(fpsText), "FPS: %.2f", fps);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
        glLoadIdentity();
        gluOrtho2D(0, glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT), 0);
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
            glLoadIdentity();
            glRasterPos2f(10, 20); // Adjust the position as needed

            for (char* c = fpsText; *c != '\0'; c++) {
                glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
            }
        glPopMatrix();
        glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}