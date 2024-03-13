#ifndef FLOOR_H
#define FLOOR_H

#include "Drawable.h"
#include <GL/freeglut.h>
#include <algorithm>

class Floor : public Drawable {
    public:
        Floor(Material* material, float lineColor[3]) : Drawable(material) {
            std::copy(lineColor, lineColor+3, this->lineColor);
        }
        void drawObject() override;
    private:
        float lineColor[3];
};

#endif