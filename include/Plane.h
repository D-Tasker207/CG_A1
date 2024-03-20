#ifndef FLOOR_H
#define FLOOR_H

#include "Drawable.h"
#include "utils.h"
#include <GL/freeglut.h>
#include <algorithm>

class Plane : public Drawable {
    public:
        Plane(Material* material, float components[4], float xRange[2], float zRange[2], float lineColor[4]);
        void draw() override;
        void drawShadows(float shadowColor[4]) override;
        void setNormal(float components[4]){
            this->A = components[0];
            this->B = components[1];
            this->C = components[2];
            this->D = components[3];
        }
        float* getComponents() { return new float[4] {A, B, C, D}; };
    private:
        float calculateY(float x, float z) { return (-D - A*x - C*z) / B; };

        float A;
        float B;
        float C;
        float D;
        float xRange[2];
        float zRange[2];
        float lineColor[4];
};

#endif