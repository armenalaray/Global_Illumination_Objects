/* date = March 20th 2021 2:47 pm */

#ifndef INTEGRATOR_H
#define INTEGRATOR_H

#include "element.h"
#include "ray.h"

class Integrator{
    public:
    virtual Element request_element(Ray r, float tMin, float tMax) = 0;
};

#endif //INTEGRATOR_H
