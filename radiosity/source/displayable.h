/* date = March 16th 2021 4:30 pm */

/* 
class Displayable
Derived Classes: class Space
Abstract class to represent displayable objects, works as interface to Radiosity back-end.
 */

#ifndef DISPLAYABLE_H
#define DISPLAYABLE_H

#include "vec3.h"
#include "ray.h"

class Displayable
{
    public:
    virtual Color<int> request_color(Ray r, float tmin, float tmax) = 0;
};

#endif //DISPLAYABLE_H
