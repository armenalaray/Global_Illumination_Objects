/* date = March 20th 2021 2:26 pm */

#ifndef HEMI_CUBE_H
#define HEMI_CUBE_H

#include "vec3.h"

class Element;

struct HemiCube{
    HemiCube(int xc, int yc);
    int xc;
    int yc;
    int y_halfc;
    float tfs;
    float dx;
    float dy;
    float half_pw;
    float half_ph;
    float da;
    Vec3<float> vup;
    Vec3<float> other_vup;
    
    size_t x;
    size_t y;
};

#endif //HEMI_CUBE_H
