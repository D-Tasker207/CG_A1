#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#endif

#include <GL/freeglut.h>
#include <cmath>
#include <stdio.h>
#include <iostream>
#include "utils.h"
#include "UFO.h"
#include "Factory.h"
#include "Crate.h"
#include "Alien.h"
#include "SkyDome.h"
#include "Plane.h"

// camera variables
float camX = 10, camY = 2, camZ = 0; //Camera position
float camRot = 0;
int camRotAmount = 10;
int camMoveAmount = 2;

// Scene Objects
std::vector<Crate*> crates;
std::vector<Alien*> groundAliens;
Alien* ufoAlien;
Plane* floorPlane;
SkyDome* skyDome;
UFO* ufo;
Factory* factory;

// Animation Variables
float takeOffHeight = 0;
int alienWalkingAngle = 0;

// Flags
bool ufoTakingOff = false;

void createSceneObjects(){
    floorPlane = new Plane(new Material(
            new float[4] {0.6, 0.6, 0.6, 1.0}, // Ambient
            new float[4] {0.6, 0.6, 0.6, 1.0}, // Diffuse
            new float[4] {.0, 0.0, 0.0, 1.0}, // Specular
            0.0), // Shininess
        new float[4]{0.0, 1.0, 0.0, 0.0}, // Plane Coefficients
        new float[2]{-50.0, 50.0}, // X Range
        new float[2]{-50.0, 50.0} // Z Range
    );
    floorPlane->addTexture("floor", new Texture("sand_02_diff_1k.jpg"));
    floorPlane->setTextureScaling(10.0, 10.0);
    
    for (int i = 0; i < 4; i++){
        Alien* alien = new Alien();
        alien->setArmsPose(75, 75, new float[3] {0, 1, 0.2}, new float[3] {0, 1, 0.2}, 60, 60, 90, 90);
        alien->setAnimPhase(getRandomFloat(0, 6));
        alien->setIsIdle(false);
        groundAliens.push_back(alien);

        crates.push_back(new Crate());
    }

    ufoAlien = new Alien();
    ufoAlien->setArmsPose(75, 75, new float[3] {0, 1, 0.2}, new float[3] {0, 1, 0.2}, 30, 30, 90, 90);
    ufoAlien->setAnimPhase(getRandomFloat(0, 6));

    ufo = new UFO();
    skyDome = new SkyDome();
    factory = new Factory(GL_LIGHT1);
}

void destroySceneObjects(){
    delete ufoAlien;
    delete ufo;
    delete floorPlane;
    delete skyDome;
    delete factory;
    ufoAlien = nullptr;
    ufo = nullptr;
    floorPlane = nullptr;
    skyDome = nullptr;
    factory = nullptr;
}

void generalIdleAnim(int value){
    value = (value + 1) % 100;
    ufo->incDishAngle();
    ufoAlien->setIdleHeight(value);
    factory->updateSpotlightAngle(value);
    for (Alien* alien : groundAliens){
        if (alien->getIsIdle()) alien->setIdleHeight(value);
        else alien->setWalkingFrame(value);
    }
    if (takeOffHeight == 0) alienWalkingAngle = (alienWalkingAngle + 1) % 360;
    glutTimerFunc(50, generalIdleAnim, value + 1);
}

void ufoTakeOffAnim(int value){
    const int scale = 50;
    takeOffHeight = pow(((float) value / scale), 2) / 6.433;

    ufo->updateSmoke();

    if (value < 2000) glutTimerFunc(15, ufoTakeOffAnim, value + 1);
}
// other animation callbacks can be added here

void startIdleAnimations(){
    glutTimerFunc(50, generalIdleAnim, 0);

    std::cout << "Idle Animations Started" << std::endl;

    // other idle animation callbacks can be added here
}

void display(void){ 
    float lpos[4] = {-2., 5., 5., 0.0};  //light's position
    float shadowMat[16];
    getShadowMat(lpos, floorPlane->getComponents(), shadowMat);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(  camX, camY, camZ, //Camera Pos 
                camX + sin(toRadians(camRot)), camY, camZ + cos(toRadians(camRot)),    //Camera LookAt
                0, 1, 0);   //Up vector

    glLightfv(GL_LIGHT0,GL_POSITION, lpos);   //Set light position

    displayFPS();

    glCullFace(GL_BACK);
    glEnable(GL_TEXTURE_2D);

    glPushMatrix(); // Draw Floor
        glTranslatef(0, -0.01, 0.0);
        floorPlane->getTexture("floor")->bind(0);
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
        floorPlane->draw();
    glPopMatrix();

    glPushMatrix(); // Draw Sky
        skyDome->draw();
    glPopMatrix();

    glDisable(GL_TEXTURE_2D);

    // Draw Factory
    
    glPushMatrix();
        glTranslatef(20, 0, 10);
        glRotatef(-60, 0, 1, 0);
        glScalef(0.3, 0.3, -0.3);
        factory->draw();
    glPopMatrix();
    

    // Draw UFO
    glPushMatrix();
        glTranslatef(0, takeOffHeight, 0);
        glPushMatrix();
            glTranslatef(0, 4.7, 0);
            glRotatef(90, 0, 1, 0);
            ufoAlien->draw();
        glPopMatrix();
        glPushMatrix();
            glScalef(0.8, 0.8, 0.8);
            ufo->draw();
        glPopMatrix();
    glPopMatrix();

    // Draw Ground Aliens
    glPushMatrix();
        glTranslatef(13, 0.75, 10);
        for (int i = 0; i < 4; i++){
            glPushMatrix();
                glRotatef(-alienWalkingAngle, 0, 1, 0);
                glRotatef(i * 90, 0, 1, 0);
                glTranslatef(10, 0, 0);
                groundAliens[i]->draw();
                glRotatef(2, 0, 1, 0);
                glTranslatef(0, 0, 1);
                glScalef(0.5, 0.5, 0.5);
                crates[i]->draw();
            glPopMatrix();
        }
    glPopMatrix();

    // Draw Shadows
    glCullFace(GL_FRONT);
    glDisable(GL_LIGHTING);
    glEnable(GL_BLEND);
    glPushMatrix();
        float shadowColor[4] = {0.1, 0.1, 0.11, 0.8};
        glMultMatrixf((float*)shadowMat);
        glPushMatrix();
            glTranslatef(0, takeOffHeight, 0);
            ufo->drawShadows(shadowColor);
        glPopMatrix();
        glPushMatrix();
            glTranslatef(13, 0.75, 10);
            for (int i = 0; i < 4; i++){
                glPushMatrix();
                    glRotatef(-alienWalkingAngle, 0, 1, 0);
                    glRotatef(i * 90, 0, 1, 0);
                    glTranslatef(10, 0, 0);
                    groundAliens[i]->drawShadows(shadowColor);
                    glRotatef(2, 0, 1, 0);
                    glTranslatef(0, 0, 1);
                    glScalef(0.5, 0.5, 0.5);
                    crates[i]->drawShadows(shadowColor);
                glPopMatrix();
            }
        glPopMatrix();
    glPopMatrix();
    glEnable(GL_LIGHTING);
    glDisable(GL_BLEND);

	glutSwapBuffers(); 
    glutPostRedisplay();
}

void initialize(void){
    createSceneObjects();

    glClearColor(0.5, 0.5, 0.5, 1.0);
    glClearDepth(1.0);
    glEnable(GL_LIGHTING);		//Enable OpenGL states
    glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    glEnable(GL_MULTISAMPLE);

    glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(30, 1.77, 10.0, 1000.0);   //Camera Frustum

    std::cout << "Initialization Complete" << std::endl;
}

void specialKeyHandler(int k, int x, int y){
    switch(k){
        case GLUT_KEY_LEFT:
            camRot += camRotAmount;
            if (camRot <= 0) camRot += 360;
            break;
        case GLUT_KEY_RIGHT:
            camRot -= camRotAmount;
            if (camRot >= 360) camRot -= 360;
            break;
        case GLUT_KEY_UP:
            camX += camMoveAmount * sin(toRadians(camRot));
            camZ += camMoveAmount * cos(toRadians(camRot));
            break;
        case GLUT_KEY_DOWN:
            camX -= camMoveAmount * sin(toRadians(camRot));
            camZ -= camMoveAmount * cos(toRadians(camRot));
            break;
        default:
            return;
    }
}

void keyHandler(unsigned char key, int x, int y){
    switch(key){
        case 27:
            destroySceneObjects();
            exit(0);
            break;
        case 'a':
            camY += 0.2;
            break;
        case 'z':
            camY -= 0.2;
            break;
        case ' ':
            if (!ufoTakingOff){
                ufo->setTakingOff(true);
                glutTimerFunc(10, ufoTakeOffAnim, 0);
            }
            break;
        default:
            return;
    }
}

void reshape(int w, int h){
	if(h == 0) h = 1;
	float ratio = 1.0* w / h;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, w, h);
	gluPerspective(45,ratio,1,1000);
	glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv){
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_MULTISAMPLE);
    glutInitWindowSize(1920, 1080);
    glutCreateWindow("Assignment 1");
    glutDisplayFunc(display);
    glutKeyboardFunc(keyHandler);
    glutSpecialFunc(specialKeyHandler);
    glutReshapeFunc(reshape);
    initialize();
    startIdleAnimations();
    glutMainLoop();
}
