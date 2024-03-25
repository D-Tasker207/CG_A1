#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#endif

#include "Particle.h"
#include "utils.h"
#include <GL/freeglut.h>
#include <cmath>
using namespace std;

Particle::Particle(float startPos[3], float velocity[3], float size, float color, int averageLifetime, Texture* texture){
    std::copy(startPos, startPos + 3, this->startPos);
    std::copy(velocity, velocity + 3, this->velocity);
    this->size = size;
    this->color = color;
    this->averageLifetime = averageLifetime;
    this->texture = texture;

    tick = 0;
}

void Particle::drawParticles(){
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_2D);
	texture->bind(0);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    for (list<particle>::iterator it = particleList.begin(); it != particleList.end(); it++){
        drawParticle(*it);
    }

    glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_DEPTH_TEST);
}

void Particle::drawParticle(particle particle){
    glColor3f(particle.col, particle.col, particle.col);
    glPushMatrix();
		glTranslatef(particle.pos[0], particle.pos[1], particle.pos[2]);
		glScalef(particle.size, particle.size, particle.size);

		glBegin(GL_QUADS);
		//A quad on the xy-plane
			glTexCoord2f(0, 0);
			glVertex3f(-0.5, -0.5, 0);
			glTexCoord2f(1, 0);
			glVertex3f(0.5, -0.5, 0);
			glTexCoord2f(1, 1);
			glVertex3f(0.5, 0.5, 0);
			glTexCoord2f(0, 1);
			glVertex3f(-0.5, 0.5, 0);

		//A quad on the yz-plane
			glTexCoord2f(0, 0);
			glVertex3f(0, -0.5, -0.5);
			glTexCoord2f(1, 0);
			glVertex3f(0, 0.5, -0.5);
			glTexCoord2f(1, 1);
			glVertex3f(0, 0.5, 0.5);
			glTexCoord2f(0, 1);
			glVertex3f(0, -0.5, 0.5);
		glEnd();
	glPopMatrix();
}

void Particle::updateParticles(){
    float delta;
    for (list<particle>::iterator it = particleList.begin(); it != particleList.end(); it++){
        delta = (float)it->currentLifeTicks / (float)it->maxLifeTicks;
        it->currentLifeTicks++;
        for (int i = 0; i < 3; i++){
            it->pos[i] += it->vel[i];
        }
        it->size = delta * 20 + size;
        it->col = color * 1 - delta;
        if (it->currentLifeTicks >= it->maxLifeTicks){
            it = particleList.erase(it);
        }
    }
    if (tick % 2 == 0) newParticle();
}

void Particle::newParticle(){
    particle newParticle;
    newParticle.currentLifeTicks = 0;
    newParticle.maxLifeTicks = averageLifetime + getRandomFloat(averageLifetime * 0.75, averageLifetime * 1.25);
    newParticle.pos[0] = startPos[0] + getRandomFloat(-1, 1);
    newParticle.pos[1] = startPos[1];
    newParticle.pos[2] = startPos[2] + getRandomFloat(-1, 1);

    newParticle.vel[0] = velocity[0] + getRandomFloat(-0.03, 0.03);
    newParticle.vel[1] = velocity[1] + getRandomFloat(-0.03, 0.03);
    newParticle.vel[2] = velocity[2] + getRandomFloat(-0.03, 0.03);

    particleList.push_back(newParticle);
}

void Particle::incTick(){
    tick++;
    if (tick == INT_MAX) tick = 0;
}