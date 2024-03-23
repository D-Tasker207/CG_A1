#ifndef FLOOR_H
#define FLOOR_H

#include "Drawable.h"

class Plane : public Drawable {
    public:
        Plane(Material* material, float components[4], float xRange[2], float zRange[2]);
        void draw() override;
        void drawShadows(float shadowColor[4]) override;
        void setTextureScaling(float uScale, float vScale);
        void setComponents(float components[4]);
        float* getComponents() { return new float[4] {A, B, C, D}; };
    private:
        float calculateY(float x, float z) { return (-D - A*x - C*z) / B; };

        float A;
        float B;
        float C;
        float D;
        float uScale;
        float vScale;
        float xRange[2];
        float zRange[2];
        float lineColor[4];
};

#endif