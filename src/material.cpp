#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#endif

#include "Material.h"
#include <GL/freeglut.h>    


Material::Material(float ambient[4], float diffuse[4], float specular[4], float shininess) {
    for (int i = 0; i < 4; i++) {
        this->ambient[i] = ambient[i];
        this->diffuse[i] = diffuse[i];
        this->specular[i] = specular[i];
    }
    this->shininess = shininess;
}

void Material::apply() {
    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, &shininess);
}