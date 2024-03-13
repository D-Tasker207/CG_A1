#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#endif

#include "Floor.h"
#include <GL/freeglut.h>

void Floor::drawObject() {
    glColor3f(lineColor[0], lineColor[1], lineColor[2]);			//Floor colour

    glDisable(GL_LIGHTING);
        for(int i = -50; i <= 50; i ++)
        {
            glBegin(GL_LINES);			//A set of grid lines on the xz-plane
                glVertex3f(-50, 0, i);
                glVertex3f(50, 0, i);
                glVertex3f(i, 0, -50);
                glVertex3f(i, 0, 50);
            glEnd();
        }
    glEnable(GL_LIGHTING);

    glBegin(GL_QUADS);
        for(int i = -50; i <= 50; i++){
            for(int j = -50; j <= 50; j++){
                glTexCoord2f(i/100.0f, j/100.0f);
                glVertex3f(i, 0, j);
                
                glTexCoord2f(i/100.0f, (j+1)/100.0f);
                glVertex3f(i, 0, j+1);
                
                glTexCoord2f((i+1)/100.0f, (j+1)/100.0f);
                glVertex3f(i+1, 0, j+1);
                
                glTexCoord2f((i+1)/100.0f, j/100.0f);
                glVertex3f(i+1, 0, j);
            }
        }
    glEnd();

}