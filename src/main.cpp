#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#endif

#include <GL/freeglut.h>
#include <cmath>
#include <stdio.h>
#include "utils.h"

// camera variables
int camX = 10, camY = 0, camZ = 0; //Camera position
float camRot = 0;
int camRotAmount = 5;
int camMoveAmount = 2;

// animation variables
// TODO: add additional animation models
float teapotAngle = 0.0;  //Rotation angle of teapot
int teapotRotationSpeed = 5;

//Draw two tris as floor plane
void drawFloor()
{
    // glColor3f(0., 0.5,  0.);			//Floor colour

    // glBegin(GL_TRIANGLES);
    //     glVertex3f(-50, -0.75, -50);
    //     glVertex3f(-50, -0.75, 50);
    //     glVertex3f(50, -0.75, 50);
    // glEnd();

    // glBegin(GL_TRIANGLES);
    //     glVertex3f(-50, -0.75, -50);
    //     glVertex3f(50, -0.75, -50);
    //     glVertex3f(50, -0.75, 50);
    // glEnd();

    glColor3f(0., 0.5,  0.);			//Floor colour

	for(int i = -50; i <= 50; i ++)
	{
		glBegin(GL_LINES);			//A set of grid lines on the xz-plane
			glVertex3f(-50, -0.75, i);
			glVertex3f(50, -0.75, i);
			glVertex3f(i, -0.75, -50);
			glVertex3f(i, -0.75, 50);
		glEnd();
	}
}

// FPS Counter Variables
int frameCount = 0;
float fps = 0;
int currentTime = 0, previousTime = 0;

void computeFPS() {
	frameCount++;
	currentTime = glutGet(GLUT_ELAPSED_TIME);
	int timeInterval = currentTime - previousTime;

	if (timeInterval > 1000) {
		fps = frameCount / (timeInterval / 1000.0f);
		previousTime = currentTime;
		frameCount = 0;
	}
}

void displayFPS() {
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

void display(void) 
{ 
	float lpos[4] = {0., 10., 10., 1.0};  //light's position

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

    gluLookAt(  camX, camY, camZ, //Camera Pos 
                camX + sin(toRadians(camRot)), camY, camZ + cos(toRadians(camRot)),    //Camera LookAt
                0, 1, 0);   //Up vector

	glLightfv(GL_LIGHT0,GL_POSITION, lpos);   //Set light position

	displayFPS();

	glDisable(GL_LIGHTING);			//Disable lighting when drawing floor.
    drawFloor();

	glEnable(GL_LIGHTING);			//Enable lighting when drawing the teapot
    glColor3f(0.0, 1.0, 1.0);
	glRotatef(60, 1,0,0);
	glRotatef(teapotAngle, 0,1,0);
    glTranslatef(2, 0, 0); // TODO: translate the teapot to a new location
    glutSolidTeapot(1.0);

	glutSwapBuffers(); 
}

void initialize(void)
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    glEnable(GL_LIGHTING);		//Enable OpenGL states
    glEnable(GL_LIGHT0);
 	glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    glEnable(GL_MULTISAMPLE);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(30, 1, 10.0, 1000.0);   //Camera Frustum
}

void keyHandler(int k, int x, int y){
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
    glutInitWindowSize(800, 800);
    glutCreateWindow("Teapot");
    glutDisplayFunc(display);
    glutIdleFunc(display);
    glutSpecialFunc(keyHandler);
    glutReshapeFunc(reshape);
    initialize();
    glutMainLoop();
}
