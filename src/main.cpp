#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#endif

#include <GL/freeglut.h>
#include <cmath>
#include <stdio.h>
#include <iostream>
#include "utils.h"
#include "Teapot.h"
// #include "Plane.h"
#include "Material.h"

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
Teapot teapot1(new Material(
    new float[4] {0.2, 0.0, 0.0, 1.0}, // Ambient color
    new float[4] {0.8, 0.0, 0.0, 1.0}, // Diffuse
    new float[4] {0.0, 0.0, 0.0, 1.0}, // Specular
    0.0 // Shininess
));

Teapot teapot2(new Material(
    new float[4] {0.0, 0.0, 0.2, 1.0}, // Ambient
    new float[4] {0.0, 0.0, 0.8, 1.0}, // Diffuse
    new float[4] {0.5, 0.5, 0.5, 1.0}, // Specular
    10.0 // Shininess
));

Teapot teapot3(new Material(
    new float[4] {0.0, 0.2, 0.0, 1.0}, // Ambient
    new float[4] {0.0, 0.8, 0.0, 1.0}, // Diffuse
    new float[4] {1.0, 1.0, 1.0, 1.0}, // Specular
    128.0 // Shininess
));

// Plane floorPlane(new Material(
//         new float[4] {0.4, 0.4, 0.4, 1.0}, // Ambient
//         new float[4] {0.4, 0.4, 0.4, 1.0}, // Diffuse
//         new float[4] {0.0, 0.0, 0.0, 1.0}, // Specular
//         0.0), // Shininess
//     new float[4]{0.0, 1.0, 0.0, 0.0}, // Plane Coefficients
//     new float[2]{-50.0, 50.0}, // X Range
//     new float[2]{-50.0, 50.0}, // Z Range
//     new float[3]{0.0, 0.8, 0.0} // Line Color
// );

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

void display(void){ 
    float lpos[4] = {2., 5., 5., 1.0};  //light's position
    float **shadowMat, **shadowMatTransposed;
    getShadowMat(lpos, new float[4] {0, 1, 0, 0}, shadowMat); //floorPlane.getComponents()
    transpose(shadowMat, shadowMatTransposed);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(  camX, camY, camZ, //Camera Pos 
                camX + sin(toRadians(camRot)), camY, camZ + cos(toRadians(camRot)),    //Camera LookAt
                0, 1, 0);   //Up vector
    // gluLookAt(5, 5, 5, 0, 0, 0, 0, 1, 0);

    glLightfv(GL_LIGHT0,GL_POSITION, lpos);   //Set light position
    glLightfv(GL_LIGHT0,GL_DIFFUSE, new float[3] {1.0, 1.0, 1.0});    //Set light colour

    displayFPS();

    glCullFace(GL_BACK);

    glPushMatrix();
        glTranslatef(0, -0.01, 0.0);
        // floorPlane.draw();
    glPopMatrix();
    glPushMatrix();
        glTranslatef(2, 2, 0); // TODO: translate the teapot to a new location
        // glRotatef(60, 1,0,0);
        glTranslatef(0, 0, -5);
        teapot1.draw();
        glTranslatef(0, 0, 5);
        teapot2.draw();
        glTranslatef(0, 0, 5);
        teapot3.draw();
    glPopMatrix();


    glCullFace(GL_FRONT);

    glDisable(GL_LIGHTING);
    glPushMatrix();
        float shadowColor[4] = {0.2, 0.2, 0.22, 1};
        glMultMatrixf((float*)shadowMatTransposed);
        glTranslatef(2, 2, 0); // TODO: translate the teapot to a new location
        // glRotatef(60, 1,0,0);
        glTranslatef(0, 0, -5);
        teapot1.drawShadows(shadowColor);
        glTranslatef(0, 0, 5);
        teapot2.drawShadows(shadowColor);
        glTranslatef(0, 0, 5);
        teapot3.drawShadows(shadowColor);
    glPopMatrix();
    glEnable(GL_LIGHTING);

    for (int i = 0; i < 4; ++i)
        delete[] shadowMat[i];
    delete[] shadowMat;

	glutSwapBuffers(); 
}

void initialize(void){
    glClearColor(0.0, 0.0, 0.0, 1.0);

    glEnable(GL_LIGHTING);		//Enable OpenGL states
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CW);

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
            exit(0);
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
    glutIdleFunc(display);
    glutKeyboardFunc(keyHandler);
    glutSpecialFunc(specialKeyHandler);
    glutReshapeFunc(reshape);
    initialize();
    glutMainLoop();
}
