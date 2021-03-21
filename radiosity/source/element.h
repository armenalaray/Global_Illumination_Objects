/* date = March 20th 2021 2:51 pm */

#ifndef ELEMENT_H
#define ELEMENT_H

#if defined(_MSC_VER)
#define _USE_MATH_DEFINES // for C++
#include <cmath>
#endif

#include "vec3.h"
#include "ray.h"
#include "matrix.h"
#include "hemi_cube.h"
#include "space.h"


extern Space space;
using ElemIndex = typename int;

class Element{
    Element()=default;
    void calc_ff(Matrix<float,2>& ffm);
    
    private:
    Vec3<float> n;
    Vec3<float> p;
    ElemIndex i;
    HemiCube hm;
};

#endif //ELEMENT_H
