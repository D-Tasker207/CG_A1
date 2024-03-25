#ifndef PARTICLE_H
#define PARTICLE_H

#include "Texture.h"
#include <list>
#include <GL/freeglut.h>
using namespace std;

struct particle {
	int currentLifeTicks; // How long particle has been alive
    int maxLifeTicks; // Maximum number of ticks until particle dies
	float col;		//Color  (0 - 1)
	float size;		//Size   (5 - 25)
	float pos[3];	//Position
	float vel[3];	//Velocity
};

class Particle {
    public:
        Particle(float startPos[3], float velocity[3], float size, float color, int averageLifetime, Texture* texture);
        void drawParticles();
        void updateParticles();
        void incTick();
    private:
        void drawParticle(particle particle);
        void newParticle();

        int tick;

        Texture* texture;
        float startPos[3];
        float velocity[3];
        float size;
        float color;
        int averageLifetime;

        list<particle> particleList;
};

#endif