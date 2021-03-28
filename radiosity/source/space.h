/* date = March 16th 2021 2:46 pm */

#ifndef SPACE_H
#define SPACE_H

/* 
class Space
referenced by: main.cpp
 Inherits From: class Displayable

The Space Object is the interface to the radiosity solver, it inherits from Displayable interface, which dispatches 
Rendering requests from multiple rendering systems. 
e.g. I have two Rendering systems in main.cpp that can request color data to space using request_color function.
PerspectiveProjector and OrthoProjector.
*/

#include <vector>
#include <memory>

#include "displayable.h"
#include "ray.h"
#include "radiosity.h"

class Space : public Displayable
{
    public:
    Space(float fw, int hps);
    // NOTE(Alex): Displayable override
    Color<int> request_color(Ray r, float tMin, float tMax) override;
    private:
    Radiosity r;
};


#endif //SPACE_H
