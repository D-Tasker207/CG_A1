#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#endif

#include <Factory.h>
#include <GL/freeglut.h>


Factory::Factory(int lightID) : q(gluNewQuadric()) {
	gluQuadricDrawStyle(q, GLU_FILL);
    gluQuadricNormals(q, GLU_SMOOTH);
    gluQuadricOrientation(q, GLU_OUTSIDE);
    gluQuadricTexture(q, GL_TRUE);

    this->lightID = lightID;
    glEnable(lightID);
    glLightfv(lightID, GL_DIFFUSE, new float[3] {1.0, 1.0, 1.0});
	glLightfv(lightID, GL_SPECULAR, new float[3] {1.0, 1.0, 1.0});
	glLightf(lightID, GL_SPOT_CUTOFF, 30.0);
	glLightf(lightID, GL_SPOT_EXPONENT, 0.0);
    
    addMaterial("shed", new Material(
        new float[4] {0.92, 0.2, 0.14, 1.0},
        new float[4] {0.92, 0.2, 0.14, 1.0},
        new float[4] {0.3, 0.3, 0.3, 1.0},
        10.0
    ));

    addMaterial("smokeStack", new Material(
        new float[4] {0.7, 0.7, 0.7, 1.0},
        new float[4] {0.7, 0.7, 0.7, 1.0},
        new float[4] {0.7, 0.7, 0.7, 1.0},
        100.0
    ));

	addMaterial("searchLight", new Material(
		new float[4] {1.0, 1.0, 1.0, 1.0},
		new float[4] {1.0, 1.0, 1.0, 1.0},
		new float[4] {1.0, 1.0, 1.0, 1.0},
		10.0
	));
}

void Factory::draw(){
	glDisable(GL_CULL_FACE);
    getMaterial("smokeStack")->apply();
    drawSmokeStack();

	getMaterial("searchLight")->apply();
	drawSearchLight();

	getMaterial("shed")->apply();
	drawShed();

	glEnable(GL_CULL_FACE);
}

void Factory::drawShadows(float shadowColor[4]){
	glColor4fv(shadowColor);

	drawSmokeStack();
	drawShed();
}

void Factory::drawSmokeStack(){
    glPushMatrix();
	   glTranslatef(0, 10, 0);
	   glRotatef(-90.0, 1., 0., 0.);
	   gluCylinder(q, 5, 4, 80, 20, 5);
	glPopMatrix();
}

void Factory::drawShed(){
	//Factory shed
	glColor3ub(235, 51, 36);
	glBegin(GL_QUAD_STRIP);
		glNormal3f(1, 0, 0);
		glVertex3f(50, 0, 40);
		glVertex3f(50, 0, -40);
		glVertex3f(50, 30, 40);
		glVertex3f(50, 30, -40);
		glNormal3f(-1, 1, 0);
		glVertex3f(7, 20, 40);
		glVertex3f(7, 20, -40);
		glNormal3f(-1, 0, 0);
		glVertex3f(7, 10, 40);
		glVertex3f(7, 10, -40);
		glNormal3f(0, 1, 0);
		glVertex3f(-7, 10, 40);
		glVertex3f(-7, 10, -40);
		glNormal3f(-1, 0, 0);
		glVertex3f(-7, 0, 40);
		glVertex3f(-7, 0, -40);
	glEnd();


	glBegin(GL_QUADS);
		glNormal3f(0, 0, 1);
		glVertex3f(50, 0, 40);
		glVertex3f(50, 30, 40);
		glVertex3f(7, 20, 40);
		glVertex3f(7, 0, 40);
		glVertex3f(7, 0, 40);
		glVertex3f(7, 10, 40);
		glVertex3f(-7, 10, 40);
		glVertex3f(-7, 0, 40);
	glEnd();
}

void Factory::drawSearchLight(){
	glPushMatrix();
		glTranslatef(-7.5, 20, 0);
		glRotatef(90, 1, 0, 0);
		glRotatef(135, 0, 1, 0);
		glTranslatef(0, 0, 3);
		glRotatef(spotlightAngle, 1, 0, 0);
		glTranslatef(0, 0, -3);
		glutSolidCone(1, 3, 20, 20);
		// glLightfv(lightID, GL_POSITION, spotlightPos);
        // glLightfv(lightID, GL_SPOT_DIRECTION, spotlightDir);
	glPopMatrix();
	
}

void Factory::updateSpotlightAngle(int keyframe){
	spotlightAngle = 20 * sin(M_2_PI * keyframe * 0.1);
}