#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#endif

#include "Drawable.h"
#include "Material.h"
#include <GL/freeglut.h>

/*
* MUST ENABLE LIGHTING BEFORE FUNCTION CALL
* MUST ENABLE TEXTURE BEFORE FUNCTION CALL IF TEXTURE IS USED
*/
void Drawable::draw() {
    material->apply();
    drawObject();
}

void Drawable::setMaterial(Material* material) {
    this->material = material;
}

void Drawable::addTexture(char* imgPath) {
    textures.push_back(new Texture(imgPath));
}

Texture* Drawable::getTexture(unsigned int unit) {
    if (unit < textures.size()) {
        return textures[unit];
    }
    return nullptr;
}

/*
* Draw the object with a shadow color
* MUST DISABLE LIGHTING BEFORE FUNCTION CALL
*/
void Drawable::drawShadows(float shadowColor[4]) {
        glColor4fv(shadowColor);
        drawObject();
}