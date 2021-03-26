/* date = March 20th 2021 2:26 pm */

/* 
struct HemiCube
 referenced by: struct Element_impl 
 Computes Form Factor Calculation.

References:
COHEN, M.F., AND GREENBERG, D.P. The hemi-cube: A radiosity solution for complex environments. Computer Graphics(SIGGRAPH '85 proceedings) 19:3 (July 1985), pp. 31-40.
*/


#ifndef HEMI_CUBE_H
#define HEMI_CUBE_H

#include "vec3.h"

class Element;

struct HemiCube{
    HemiCube(){}
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
