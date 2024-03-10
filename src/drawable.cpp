#include "Drawable.h"
#include "Material.h"
#include <GL/freeglut.h>

void Drawable::draw() {
    // Set the material properties
    material->apply();

    // Draw the object
    drawObject();
}

void Drawable::setMaterial(Material* material) {
    this->material = material;
}

void Drawable::drawShadows(float shadowColor[4]) {
    // Save the current material
    Material* oldMaterial = material;

    // Create a new material with the shadow color
    Material shadowMaterial(
        shadowColor, // ambient color
        shadowColor, // diffuse color
        new float[4]{0.0, 0.0, 0.0, 1.0}, // specular color
        0.0 // shininess
    );

    // Set the new material
    material = &shadowMaterial;

    glDisable(GL_LIGHTING);

    material->apply();
    drawObject();

    glEnable(GL_LIGHTING);

    // Restore the old material
    material = oldMaterial;
}