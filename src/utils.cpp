#include <cmath>
#include "utils.h"

float toRadians(float degrees){
    return degrees * (M_PI / 180);
}

float toDegrees(float radians){
    return radians * (180 / M_PI);
}