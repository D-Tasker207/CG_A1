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
void computeVertexNormals(float x[], float y[], float vnx[], float vny[], int n);
void drawRevolution(float vx[], float vy[], float vz[], int nPoints, float angStep, NormalDirection normalDirection);

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

void computeVertexNormals(float x[], float y[], float vnx[], float vny[], int n){
    float nx, ny, nz, dist;

    for (int i = 0; i < n; i++)
    {
        if (i == 0){// End point of the curve
            nx = y[1] - y[0];
            ny = -x[1] + x[0];
        }
        else if (i == n - 1){ // End point of the curve
            nx = y[i] - y[i - 1];
            ny = -x[i] + x[i - 1];
        }
        else{ // All interior points
            nx = y[i + 1] - y[i - 1];  // x-component of n1+n2
            ny = -x[i + 1] + x[i - 1]; // y-component of n1+n2
        }
        dist = sqrt(nx * nx + ny * ny); // normalization
        nx /= dist;
        ny /= dist;
        vnx[i] = nx; // Store values in an array
        vny[i] = ny;
    }
}

static void vertexNormal(float nx, float ny, float nz, NormalDirection normalDirection){
    switch(normalDirection){
    case NORMAL:
        glNormal3f(nx, ny, nz);
        break;
    case REVERSED:
        glNormal3f(-nx, -ny, -nz);
        break;
    }
}

void drawRevolution(float vx[], float vy[], float vz[], int nPoints, float angStep, NormalDirection normalDirection){
    float wx[nPoints], wy[nPoints], wz[nPoints];

    float nx[nPoints], ny[nPoints], nz[nPoints]; 
    float mx[nPoints], my[nPoints], mz[nPoints];

    for(int i=0; i<nPoints; i++){ nz[i] = 0; } // set nz to 0 since revolution is defined in 2D

    computeVertexNormals(vx, vy, nx, ny, nPoints);

    float nSlices = 360 / angStep;
    angStep = angStep * M_PI / 180; // Convert to radians

    glPushMatrix();
    for (int j = 0; j < nSlices; j++){ // Transform verts and normals
            for (int i = 0; i < nPoints; i++){
                wx[i] = cos(angStep) * vx[i] + sin(angStep) * vz[i];
                wy[i] = vy[i];
                wz[i] = -sin(angStep) * vx[i] + cos(angStep) * vz[i];
                mx[i] = cos(angStep) * nx[i] + sin(angStep) * nz[i];
                my[i] = ny[i];
                mz[i] = -sin(angStep) * nx[i] + cos(angStep) * nz[i];
            }
            glBegin(GL_QUAD_STRIP); // Generate quad strip
                for (int i = 0; i < nPoints; i++){
                    glTexCoord2f((float)j / nSlices, (float)i / (nPoints-1));
                    vertexNormal(nx[i], ny[i], nz[i], normalDirection);
                    glVertex3f(vx[i], vy[i], vz[i]);
                    glTexCoord2f((float)(j + 1) / nSlices, (float)i / (nPoints-1));
                    vertexNormal(mx[i], my[i], mz[i], normalDirection);
                    glVertex3f(wx[i], wy[i], wz[i]);
                }
            glEnd();
            for (int i = 0; i < nPoints; i++){ // Update verts and normals
                vx[i] = wx[i];
                vy[i] = wy[i];
                vz[i] = wz[i];
                nx[i] = mx[i];
                ny[i] = my[i];
                nz[i] = mz[i];
            }
        }
    glPopMatrix();
}