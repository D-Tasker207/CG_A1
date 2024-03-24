#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#endif

#include "UFO.h"
#include <GL/freeglut.h>
#include <cmath>
#include <iostream>

static void computeRevNormals(float x[], float y[], float vnx[], float vny[], int n);

UFO::UFO() {
    addMaterial("body", new Material(
        new float[4] {0.6, 0.6, 0.6, 1.0}, // Ambient
        new float[4] {0.6, 0.6, 0.6, 1.0}, // Diffuse
        new float[4] {0.0, 0.0, 0.0, 1.0}, // Specular
        0.0 // Shininess
    ));

    addMaterial("canopy", new Material(
        new float[4] {0.678, 0.847, 0.902, 0.3}, // Ambient
        new float[4] {0.678, 0.847, 0.902, 0.3}, // Diffuse
        new float[4] {1.0, 1.0, 1.0, 0.4}, // Specular
        128.0 // Shininess
    ));

     addMaterial("antenna", new Material(
        new float[4] {0.3, 0.3, 0.3, 1.0}, // Ambient
        new float[4] {0.3, 0.3, 0.3, 1.0}, // Diffuse
        new float[4] {1.0, 1.0, 1.0, 1.0}, // Specular
        100.0 // Shininess
    ));

    addMaterial("antenna_dish", new Material(
        new float[4] {0.75, 0.75, 0.75, 1.0}, // Ambient
        new float[4] {0.75, 0.75, 0.75, 1.0}, // Diffuse
        new float[4] {1.0, 1.0, 1.0, 1.0}, // Specular
        128.0 // Shininess
    ));

    addMaterial("lights", new Material(
        new float[4] {0.8, 0.8, 0.0, 1.0}, // Ambient
        new float[4] {0.8, 0.8, 0.0, 1.0}, // Diffuse
        new float[4] {1.0, 1.0, 1.0, 1.0}, // Specular
        64.0 // Shininess
    ));
}

void UFO::draw() {
    drawBody();
    drawCanopy();
    drawLights();
    drawAntenna();
}

void UFO::drawShadows(float shadowColor[4]) {
    
    glColor4fv(shadowColor);

    drawBody();
    drawCanopy();
    drawLights();
    drawAntenna();
}

void UFO::drawBody() {
    getMaterial("body")->apply();

    float vx[bodyRevolutionVertices], vy[bodyRevolutionVertices], vz[bodyRevolutionVertices];
    float wx[bodyRevolutionVertices], wy[bodyRevolutionVertices], wz[bodyRevolutionVertices];
    float angStep = 18;
    int nSlices = 360 / angStep;
    angStep = angStep * M_PI / 180;

    for (int i = 0; i < bodyRevolutionVertices; i++){
		vx[i] = bodyRevolutionLineX[i];
		vy[i] = bodyRevolutionLineY[i];
		vz[i] = 0;
	}

    glPushMatrix();
        glScalef(0.3, 1.0, 0.3);
        for (int j = 0; j < nSlices; j++){
            for (int i = 0; i < bodyRevolutionVertices; i++){
                wx[i] = cos(angStep) * vx[i] + sin(angStep) * vz[i];
                wy[i] = vy[i];
                wz[i] = -sin(angStep) * vx[i] + cos(angStep) * vz[i];	
            }

            glBegin(GL_QUAD_STRIP);
                for (int i = 0; i < bodyRevolutionVertices; i++){
                    float edge1[3] = {wx[i] - vx[i], wy[i] - vy[i], wz[i] - vz[i]};
                    float edge2[3] = {wx[(i+1)%bodyRevolutionVertices] - vx[i], wy[(i+1)%bodyRevolutionVertices] - vy[i], wz[(i+1)%bodyRevolutionVertices] - vz[i]};

                    glTexCoord2f((float)j / nSlices, (float)i / (bodyRevolutionVertices-1));
                    glNormal3f(edge1[1]*edge2[2] - edge1[2]*edge2[1], edge1[2]*edge2[0] - edge1[0]*edge2[2], edge1[0]*edge2[1] - edge1[1]*edge2[0]);
                    glVertex3f(vx[i], vy[i], vz[i]);
                    glTexCoord2f((float)(j + 1) / nSlices, (float)i / (bodyRevolutionVertices-1));
                    glVertex3f(wx[i], wy[i], wz[i]);
                }
            glEnd();

            for (int i = 0; i < bodyRevolutionVertices; i++){
                vx[i] = wx[i];
                vy[i] = wy[i];
                vz[i] = wz[i];
            }
        }
    glPopMatrix();
}

void UFO::drawCanopy() {
    getMaterial("canopy")->apply();

    const int nPoints = 11;

    float vx[nPoints] = {0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0};
    float vy[nPoints], vz[nPoints] = { 0.0 };
    float wx[nPoints], wy[nPoints], wz[nPoints] = { 0.0 };

    for(int i = 0; i < nPoints; i++){
        vy[i] = -pow(vx[i], 4) + 1;
    }

    float nx[nPoints], ny[nPoints], nz[nPoints] = { 0.0 };
    float mx[nPoints], my[nPoints], mz[nPoints];

    computeRevNormals(vx, vy, nx, ny, nPoints);

    float angStep = 36;
    int nSlices = 360 / angStep;
    angStep = angStep * M_PI / 180;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glPushMatrix();
        glTranslatef(0.0, 5, 0.0);
        glScalef(3, 2, 3);
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
                    glTexCoord2f((float)j / nSlices, (float)i / (bodyRevolutionVertices-1));
                    glNormal3f(nx[i], ny[i], nz[i]);
                    glVertex3f(vx[i], vy[i], vz[i]);
                    glTexCoord2f((float)(j + 1) / nSlices, (float)i / (bodyRevolutionVertices-1));
                    glNormal3f(mx[i], my[i], mz[i]);
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
    glDisable(GL_BLEND);
}

void UFO::drawLights() {
    getMaterial("lights")->apply();

    const int nLights = 40;
    const float baseEmissiveColor[4] = {1.0, 1.0, 1.0, 1.0};
    float emissiveColor[4];
    for(int i = 0; i < nLights; i++){
        for(int j = 0; j < sizeof(emissiveColor)/sizeof(emissiveColor[0]); j++){
            emissiveColor[j] = baseEmissiveColor[j] * (i/nLights);
        }
        glPushMatrix();
            glRotatef(i*360/nLights, 0, 1, 0);
            glScalef(0.5, 1, 0.5);
            glTranslatef(12.4, 3, 0);
            glMaterialfv(GL_FRONT, GL_EMISSION, emissiveColor);
            glutSolidCube(1.0);
        glPopMatrix();
    }
}

void UFO::drawAntenna() {
    getMaterial("antenna_dish")->apply();

    const int nPoints = 11;
    float vx[nPoints] = {0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0};
    float vy[nPoints], vz[nPoints] = { 0.0 };
    float wx[nPoints], wy[nPoints], wz[nPoints];

    for(int i = 0; i < nPoints; i++){
        vy[i] = 0.5 * pow(vx[i], 2);
    }

    float nx[nPoints], ny[nPoints], nz[nPoints] = { 0.0 };
    float mx[nPoints], my[nPoints], mz[nPoints];

    computeRevNormals(vx, vy, nx, ny, nPoints);

    float angStep = 36;
    int nSlices = 360 / angStep;
    angStep = angStep * M_PI / 180;
    glPushMatrix();
        glTranslatef(0.0, 5, 0.0);

        glPushMatrix();
            glScalef(0.3, 0.3, 0.3);
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
                        glTexCoord2f((float)j / nSlices, (float)i / (bodyRevolutionVertices-1));
                        glNormal3f(nx[i], ny[i], nz[i]);
                        glVertex3f(vx[i], vy[i], vz[i]);
                        glTexCoord2f((float)(j + 1) / nSlices, (float)i / (bodyRevolutionVertices-1));
                        glNormal3f(mx[i], my[i], mz[i]);
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

        getMaterial("antenna")->apply();
        glPushMatrix();

        glPopMatrix();
    glPopMatrix();
}

static void computeRevNormals(float x[], float y[], float vnx[], float vny[], int n){
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
        nx /= -dist;
        ny /= -dist;
        vnx[i] = nx; // Store values in an array
        vny[i] = ny;
    }
}