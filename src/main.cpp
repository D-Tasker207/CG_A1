#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#endif

#include <GL/freeglut.h>
#include <cmath>
#include <stdio.h>
#include <iostream>
#include "utils.h"
#include "UFO.h"
#include "Teapot.h"
#include "SkyDome.h"
#include "Plane.h"

// camera variables
float camX = 10, camY = 2, camZ = 0; //Camera position
float camRot = 0;
int camRotAmount = 15;
int camMoveAmount = 2;

// animation variables
// TODO: add additional animation models
float teapotAngle = 0.0;  //Rotation angle of teapot
int teapotRotationSpeed = 5;

// Scene Objects
Teapot** teapots;
Plane* floorPlane;
SkyDome* skyDome;
UFO* ufo;

void createSceneObjects(){
    teapots = new Teapot*[3];

    teapots[0] = new Teapot(new Material(
        new float[4] {0.2, 0.0, 0.0, 1.0}, // Ambient color
        new float[4] {0.8, 0.0, 0.0, 1.0}, // Diffuse
        new float[4] {0.0, 0.0, 0.0, 1.0}, // Specular
        0.0 // Shininess
    ));

    teapots[1] = new Teapot(new Material(
        new float[4] {0.0, 0.0, 0.2, 1.0}, // Ambient
        new float[4] {0.0, 0.0, 0.8, 1.0}, // Diffuse
        new float[4] {0.5, 0.5, 0.5, 1.0}, // Specular
        10.0 // Shininess
    ));

    teapots[2] = new Teapot(new Material(
        new float[4] {0.0, 0.2, 0.0, 1.0}, // Ambient
        new float[4] {0.0, 0.8, 0.0, 1.0}, // Diffuse
        new float[4] {1.0, 1.0, 1.0, 1.0}, // Specular
        128.0 // Shininess
    ));

    floorPlane = new Plane(new Material(
            new float[4] {0.6, 0.6, 0.6, 1.0}, // Ambient
            new float[4] {0.6, 0.6, 0.6, 1.0}, // Diffuse
            new float[4] {0.0, 0.0, 0.0, 1.0}, // Specular
            0.0), // Shininess
        new float[4]{0.0, 1.0, 0.0, 0.0}, // Plane Coefficients
        new float[2]{-50.0, 50.0}, // X Range
        new float[2]{-50.0, 50.0} // Z Range
    );
    floorPlane->setTextureScaling(10.0, 10.0);
    floorPlane->addTexture("floor", new Texture("sand_02_diff_1k.jpg"));

    skyDome = new SkyDome();

    ufo = new UFO();
}

void destroySceneObjects(){
    for (int i = 0; i < 3; i++){
        delete teapots[i];
    }
    delete[] teapots;
    delete ufo;
    delete floorPlane;
    delete skyDome;
    ufo = nullptr;
    teapots = nullptr;
    floorPlane = nullptr;
    skyDome = nullptr;
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
    // gluLookAt(5, 5, 5, 0, 0, 0, 0, 1, 0);

    glLightfv(GL_LIGHT0,GL_POSITION, lpos);   //Set light position

    displayFPS();

    glCullFace(GL_BACK);
    glEnable(GL_TEXTURE_2D);

    glPushMatrix();
        glTranslatef(0, -0.01, 0.0);
        floorPlane->getTexture("floor")->bind(0);
        floorPlane->draw();
    glPopMatrix();

    
    glPushMatrix();
        skyDome->draw();
    glPopMatrix();

    glDisable(GL_TEXTURE_2D);

    glPushMatrix();
        glTranslatef(0, 0, 0);
        ufo->draw();
    glPopMatrix();


    glCullFace(GL_FRONT);

    glDisable(GL_LIGHTING);
    glEnable(GL_BLEND);
    // glPushMatrix();
    //     float shadowColor[4] = {0.1, 0.1, 0.11, 0.8};
    //     glMultMatrixf((float*)shadowMat);
    //     glTranslatef(2, 2, 0); // TODO: translate the teapot to a new location
    //     glTranslatef(0, 0, -5);
    //     teapots[0]->drawShadows(shadowColor);
    //     glTranslatef(0, 0, 5);
    //     teapots[1]->drawShadows(shadowColor);
    //     glTranslatef(0, 0, 5);
    //     teapots[2]->drawShadows(shadowColor);
    // glPopMatrix();
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
    glEnable(GL_LIGHT0);

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
    glutMainLoop();
}
