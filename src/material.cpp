#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#endif

#include <string>
#include "Material.h"
#include <GL/freeglut.h>    


Material::Material(float ambient[4], float diffuse[4], float specular[4], float shininess) {
    std::copy(ambient, ambient + 4, this->ambient);
    std::copy(diffuse, diffuse + 4, this->diffuse);
    std::copy(specular, specular + 4, this->specular);
    this->shininess = shininess;
}

void Material::apply() {
    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, &shininess);
}