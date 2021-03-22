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

using ElemIndex = typename int;
enum class corner_it : int {tf=0,rf=1,lf=2,ff=3,bf=4,corner_index_count=5};

struct Element_data{
    Element_data():
    n{},
    p{},
    i{},
    
    u{},
    v{},
    w{},
    
    corner_i{0},
    
    corners{}
    {}
    
    Vec3<float> n;
    Vec3<float> p;
    ElemIndex i;
    
    Vec3<float> u;
    Vec3<float> v;
    Vec3<float> w;
    
    int corner_i;
    
    Vec3<float> corners[corner_it::corner_index_count];
};

class Element{
    public:
    Element();
    // NOTE(Alex): Element_base ovverides
    bool get_ray(Ray& r);
    void calc_ff(const Ray& r, const Element& j, Matrix_2d<float>& ffm);
    ElemIndex get_index()const{return data.i;}
    private:
    HemiCube hm;
    Element_data data;
};

#endif //ELEMENT_H
