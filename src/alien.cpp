#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#endif

#include "Alien.h"
#include <GL/freeglut.h>

Alien::Alien() : q(gluNewQuadric()) {
    walkingAngle = 0;
    animPhase = 0;
    idleHeight = 0;
    leftForeArmAngle = 0;
    rightForeArmAngle = 0;
    isIdle = true;

    addTexture("eyes", new Texture("eyeball.png"));
    gluQuadricDrawStyle(q, GLU_FILL);
    gluQuadricNormals(q, GLU_SMOOTH);
    gluQuadricOrientation(q, GLU_OUTSIDE);
    gluQuadricTexture(q, GL_TRUE);

    addMaterial("eyes", new Material(
        new float[4] {1.0, 1.0, 1.0, 1.0}, // Ambient
        new float[4] {1.0, 1.0, 1.0, 1.0}, // Diffuse
        new float[4] {1.0, 1.0, 1.0, 1.0}, // Specular
        10.0 // Shininess
    ));

    addMaterial("mouth", new Material(
        new float[4] {0.0, 0.0, 0.0, 1.0}, // Ambient
        new float[4] {0.0, 0.0, 0.0, 1.0}, // Diffuse
        new float[4] {0.0, 0.0, 0.0, 1.0}, // Specular
        10.0 // Shininess
    ));

    addMaterial("legs", new Material(
        new float[4] {0.647, 0.165, 0.165, 1.0}, // Ambient
        new float[4] {0.647, 0.165, 0.165, 1.0}, // Diffuse
        new float[4] {0.2, 0.2, 0.2, 1.0}, // Specular
        10.0 // Shininess
    ));

    addMaterial("feet", new Material(
        new float[4] {0.0, 0.0, 0.0, 1.0}, // Ambient
        new float[4] {0.0, 0.0, 0.0, 1.0}, // Diffuse
        new float[4] {1.0, 1.0, 1.0, 1.0}, // Specular
        10.0 // Shininess
    ));

    addMaterial("arms", new Material(
        new float[4] {0.0, 0.0, 0.75, 1.0}, // Ambient
        new float[4] {0.0, 0.0, 0.75, 1.0}, // Diffuse
        new float[4] {0.2, 0.2, 0.2, 1.0}, // Specular
        1.0 // Shininess
    ));

    addMaterial("body", new Material(
        new float[4] {1.0, 0.647, 0.0, 1.0}, // Ambient
        new float[4] {1.0, 0.647, 0.0, 1.0}, // Diffuse
        new float[4] {0.2, 0.2, 0.2, 1.0}, // Specular
        10.0 // Shininess
    ));
}

void Alien::draw(){
    glPushMatrix();
        if (isIdle) glTranslatef(0, idleHeight, 0);
        getMaterial("body")->apply();
        drawBody();

        getMaterial("mouth")->apply();
        drawMouth();

        getMaterial("arms")->apply();
        drawArms();

        glEnable(GL_TEXTURE_2D);
            getMaterial("eyes")->apply();
            getTexture("eyes")->bind(0);
            drawEyes();
        glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    getMaterial("feet")->apply();
    drawFeet();

    getMaterial("legs")->apply();
    drawLegs();
}

void Alien::drawShadows(float shadowColor[4]){
    glColor4fv(shadowColor);

    glPushMatrix();
        if (isIdle) glTranslatef(0, idleHeight, 0);
        drawBody();
        drawMouth();
        drawArms();
        drawEyes();
    glPopMatrix();

    drawFeet();
    drawLegs();
}

void Alien::drawBody(){
    glPushMatrix();
    glTranslatef(0, 0.5, 0);
    glutSolidCube(1);
    glPopMatrix();
}

void Alien::drawEyes(){
    glPushMatrix();
        glTranslatef(0.35, 1, 0.5);
        glRotatef(-90, 1, 0, 0);
        glScalef(0.2, 0.2, 0.2);
        gluSphere(q, 1, 12, 12);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(-0.35, 1, 0.5);
        glRotatef(-90, 1, 0, 0);
        glScalef(0.2, 0.2, 0.2);
        gluSphere(q, 1, 12, 12);
    glPopMatrix();
}

void Alien::drawMouth(){
    const int sides = 12;
    const float innerRadius = 2;
    const float outerRadius = 3;

    glPushMatrix();
        glTranslatef(0, 0.75, 0.51);
        glRotatef(180, 0, 0, 1);
        glScalef(0.1, 0.1, 0.1);
        glBegin(GL_QUAD_STRIP);
        for (int i = 0; i <= sides; ++i) {
            float angle = (float)i / sides * 3.14159f;
            float x = cos(angle);
            float y = sin(angle);

            glVertex2f(innerRadius * x, innerRadius * y);
            glVertex2f(outerRadius * x, outerRadius * y);
        }
        glEnd();    
    glPopMatrix();
}

void Alien::drawLegs(){
    glPushMatrix();
        glTranslatef(0.3, 0, 0);
        if (!isIdle) glRotatef(walkingAngle, 1, 0, 0);
        glTranslatef(0, -0.25, 0);
        glScalef(0.2, 0.5, 0.2);
        glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(-0.3, 0, 0);
        if (!isIdle) glRotatef(-walkingAngle, 1, 0, 0);
        glTranslatef(0, -0.25, 0);
        glScalef(0.2, 0.5, 0.2);
        glutSolidCube(1);
    glPopMatrix();
}

void Alien::drawFeet(){
    glPushMatrix();
        if (!isIdle) glRotatef(walkingAngle, 1, 0, 0);
        glTranslatef(0.3, -0.6, 0.15);
        glScalef(0.2, 0.2, 0.5);
        glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
        if (!isIdle) glRotatef(-walkingAngle, 1, 0, 0);
        glTranslatef(-0.3, -0.6, 0.15);
        glScalef(0.2, 0.2, 0.5);
        glutSolidCube(1);
    glPopMatrix();
}

void Alien::drawArms(){
    glPushMatrix();
        glTranslatef(0.5, 0.25, 0);
        glRotatef(-leftArmAngle, leftArmAxis[0], leftArmAxis[1], leftArmAxis[2]);
        glTranslatef(0.25, 0, 0);
        glPushMatrix();
            glScalef(0.5, 0.1, 0.1);
            glutSolidCube(1);
        glPopMatrix();

        glTranslatef(0.25, 0, 0);
        glPushMatrix();
            glScalef(0.1, 0.1, 0.1);
            glutSolidSphere(1, 12, 12);
        glPopMatrix();

        glRotatef(leftForeArmTwist, 1, 0, 0);
        glRotatef(leftForeArmAngle, 0, 0, 1);
        glTranslatef(0.25, 0, 0);
        glPushMatrix();
            glScalef(0.5, 0.1, 0.1);
            glutSolidCube(1);
        glPopMatrix();
    glPopMatrix();

    glPushMatrix();
        glTranslatef(-0.5, 0.25, 0);
        glRotatef(rightArmAngle, rightArmAxis[0], rightArmAxis[1], rightArmAxis[2]);
        glTranslatef(-0.25, 0, 0);
        glPushMatrix();
            glScalef(0.5, 0.1, 0.1);
            glutSolidCube(1);
        glPopMatrix();

        glTranslatef(-0.25, 0, 0);
        glPushMatrix();
            glScalef(0.1, 0.1, 0.1);
            glutSolidSphere(1, 12, 12);
        glPopMatrix();

        glRotatef(rightForeArmTwist, 1, 0, 0);
        glRotatef(-rightForeArmAngle, 0, 0, 1);
        glTranslatef(-0.25, 0, 0);
        glPushMatrix();
            glScalef(0.5, 0.1, 0.1);
            glutSolidCube(1);
        glPopMatrix();
    glPopMatrix();
}

void Alien::setIdleHeight(int keyframe){
    idleHeight = 0.01 * sin(M_2_PI * keyframe * 0.1 + animPhase);
}

void Alien::setWalkingFrame(int keyframe){
	walkingAngle = 20 * sin(M_2_PI * keyframe * 0.1 + animPhase);
}

void Alien::setIsIdle(bool isIdle){
    this->isIdle = isIdle;
}

void Alien::setArmsPose(
    float rightArmAngle, float leftArmAngle,
    float rightArmAxis[3], float leftArmAxis[3],
    float rightForeArmAngle, float leftForeArmAngle,
    float rightForeArmTwist, float leftForeArmTwist
){
    this->rightArmAngle = rightArmAngle;
    this->leftArmAngle = leftArmAngle;
    std::copy(rightArmAxis, rightArmAxis + 3, this->rightArmAxis);
    std::copy(leftArmAxis, leftArmAxis + 3, this->leftArmAxis);

    this->rightForeArmAngle = rightForeArmAngle;
    this->leftForeArmAngle = leftForeArmAngle;
    this->rightForeArmTwist = rightForeArmTwist;
    this->leftForeArmTwist = leftForeArmTwist;
}