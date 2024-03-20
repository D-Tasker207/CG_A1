#ifndef MATERIAL_H
#define MATERIAL_H

class Material {
    public: 
        Material(float ambient[4], float diffuse[4], float specular[4], float shininess);
        void apply();
    private:
        float ambient[4];
        float diffuse[4];
        float specular[4];
        float shininess;
};

#endif