#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#endif

#include "UFO.h"
#include "utils.h"
#include <GL/freeglut.h>
#include <cmath>
#include <iostream>

UFO::UFO() {
    addMaterial("body", new Material(
        new float[4] {0.73, 0.73, 0.8, 1.0}, // Ambient
        new float[4] {0.73, 0.73, 0.8, 1.0}, // Diffuse
        new float[4] {1.0, 1.0, 1.0, 1.0}, // Specular
        10.0 // Shininess
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
        new float[4] {1, 1, 1, 1.0}, // Ambient
        new float[4] {1, 1, 1, 1.0}, // Diffuse
        new float[4] {1.0, 1.0, 1.0, 1.0}, // Specular
        128.0 // Shininess
    ));

    addMaterial("lights", new Material(
        new float[4] {0.8, 0.8, 0.0, 1.0}, // Ambient
        new float[4] {0.8, 0.8, 0.0, 1.0}, // Diffuse
        new float[4] {1.0, 1.0, 1.0, 1.0}, // Specular
        64.0 // Shininess
    ));

    addTexture("dish", new Texture("rusty_metal_sheet_diff_1k.jpg"));
    
    addMaterial("legs", new Material(
        new float[4] {0.0, 0.0, 0.75, 1.0}, // Ambient
        new float[4] {0.0, 0.0, 0.75, 1.0}, // Diffuse
        new float[4] {1.0, 1.0, 1.0, 1.0}, // Specular
        0.0 // Shininess
    ));

    addMaterial("feet", new Material(
        new float[4] {0.8, 0.0, 0.0, 1.0}, // Ambient
        new float[4] {0.8, 0.0, 0.0, 1.0}, // Diffuse
        new float[4] {0.0, 0.0, 0.0, 1.0}, // Specular
        0.0 // Shininess
    ));

    q = gluNewQuadric();
    gluQuadricNormals(q, GLU_SMOOTH);
    gluQuadricDrawStyle(q, GLU_FILL);

    engineSmoke = new Particle(new float[3] {0, 1, 0}, new float[3] {0, -0.3, 0}, 5, 1, 200, new Texture("Glow.bmp"));

    takingOff = false;
    takeOffHeight = 0;
    dishAngle = 0;
}

void UFO::draw() {
    glPushMatrix();
        glTranslatef(0, takeOffHeight, 0);
        glPushMatrix();
            glTranslatef(0, 1, 0);
            drawBody();
            drawCanopy();
            drawLights();
            drawAntenna();
            glEnable(GL_TEXTURE_2D);
                drawDish();
            glDisable(GL_TEXTURE_2D);
            drawAntennaBase();
        glPopMatrix();
        drawLegs();
        if (takingOff) drawSmoke();
    glPopMatrix();
}

void UFO::drawShadows(float shadowColor[4]) {
    
    glColor4fv(shadowColor);

    glPushMatrix();
        glTranslatef(0, takeOffHeight, 0);
        drawBody();
        // drawCanopy();
        // drawLights();
        // drawAntenna();
        // drawDish();
        // drawAntennaBase();
    glPopMatrix();
}

void UFO::drawBody() {
    getMaterial("body")->apply();

    const int nPoints = 16;
    float vx[nPoints] = { 0, 5, 2.5, 2.5, 5, 10, 15, 20, 25, 25, 20, 15, 10, 7.5, 7.5, 0 };
    float vy[nPoints] = { 0, 0, 0.25, 0.5, 0.75, 1, 1.25, 1.75, 2.5, 3.5, 4.25, 4.75, 5, 5, 4, 4 };
    float vz[nPoints] = { 0.0 };

    glPushMatrix();
        glScalef(0.3, 1.0, 0.3);
        drawRevolution(vx, vy, vz, nPoints, 18, NORMAL);
    glPopMatrix();
}

void UFO::drawCanopy() {
    getMaterial("canopy")->apply();

    const int nPoints = 11;
    float vx[nPoints] = {0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0};
    float vy[nPoints], vz[nPoints] = { 0.0 };

    for(int i = 0; i < nPoints; i++){
        vy[i] = -pow(vx[i], 4) + 1;
    }

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glPushMatrix();
        glTranslatef(0.0, 5, 0.0);
        glScalef(3, 2, 3);
        drawRevolution(vx, vy, vz, nPoints, 36, REVERSED);
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
            glTranslatef(14.5, 3, 0);
            glMaterialfv(GL_FRONT, GL_EMISSION, emissiveColor);
            glutSolidCube(1.0);
        glPopMatrix();
    }
}

void UFO::drawAntenna() {  
    glPushMatrix();
        glTranslatef(6, 4.7, 0.0);
        glPushMatrix();
            glRotatef(dishAngle, 0, 1, 0);
            glRotatef(-60, 1, 0, 0);
            getMaterial("antenna")->apply();
            glPushMatrix(); // Antenna
                glRotatef(-90, 1, 0, 0);
                gluCylinder(q, 0.1, 0.05, 0.3, 6, 1);
                glTranslatef(0, 0, 0.3);
                gluDisk(q, 0, 0.05, 6, 1);
            glPopMatrix();
        glPopMatrix();
    glPopMatrix();
}

void UFO::drawDish(){
    getMaterial("antenna_dish")->apply();

    const int nPoints = 11;
    float vx[nPoints] = {0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0};
    float vy[nPoints], vz[nPoints] = { 0.0 };

    for(int i = 0; i < nPoints; i++){
        vy[i] = 0.5 * pow(vx[i], 2);
    }

    glPushMatrix();
        glTranslatef(6, 4.7, 0.0);

        glPushMatrix();
            glRotatef(dishAngle, 0, 1, 0);
            glRotatef(-60, 1, 0, 0);
            getTexture("dish")->bind(0);
            glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
            glPushMatrix(); // Dish
                glScalef(0.6, 0.6, 0.6);
                glDisable(GL_CULL_FACE);
                drawRevolution(vx, vy, vz, nPoints, 36, NORMAL);
                glEnable(GL_CULL_FACE);
            glPopMatrix();
        glPopMatrix();
    glPopMatrix();
}

void UFO::drawAntennaBase(){
    getMaterial("antenna")->apply();

    glPushMatrix();
        glTranslatef(6, 4.7, 0.0);
        glPushMatrix(); // Base
            glRotatef(90, 1, 0, 0);
            gluCylinder(q, 0.1, 0.1, 0.7, 6, 1);
        glPopMatrix();
    glPopMatrix();
}

void UFO::drawLegs(){
    glPushMatrix();
        glTranslatef(0, 4, 0);
        for(int i = 0; i < 4; i++){
            glPushMatrix();
                getMaterial("legs")->apply();
                glRotatef(90*i, 0, 1, 0);
                glTranslatef(3, 0, 0);
                glPushMatrix(); // Leg
                    glRotatef(90, 1, 0, 0);
                    glRotatef(30, 0, 1, 0);
                    gluCylinder(q, 0.3, 0.3, 4.5, 6, 1);
                glPopMatrix();
            glPopMatrix();

            getMaterial("feet")->apply();

            glPushMatrix();
                glTranslatef(0, -4, 0);
                glRotatef(90*i, 0, 1, 0);
                glPushMatrix(); // Foot
                    glTranslatef(5.5, 0, 0);
                    glScalef(1.5, 0.5, 1);
                    glutSolidCube(1.0);
                glPopMatrix();
            glPopMatrix();
        }
    glPopMatrix();
}

void UFO::drawSmoke(){
    engineSmoke->drawParticles();
}

void UFO::updateSmoke(){
    engineSmoke->incTick();
    engineSmoke->updateParticles();
}

void UFO::incDishAngle(){
    dishAngle += 1;
    if(dishAngle >= 360){
        dishAngle = 0;
    }
}

void UFO::incTakeOffHeight(int keyframe){
    if (keyframe <= 1000) takeOffHeight = (((float)keyframe / 1000) - sin(keyframe)) + 0.5 * ((float)keyframe / 1000);
    else takeOffHeight = pow(((float) keyframe / 1000), 2) / 6.433;
}