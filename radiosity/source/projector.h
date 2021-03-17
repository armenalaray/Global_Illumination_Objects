/* date = March 16th 2021 2:34 pm */

#ifndef PROJECTOR_H
#define PROJECTOR_H

#include "vec3.h"
#include "canvas.h"
#include "space.h"

class Projector{
    public:
    virtual void Render() = 0;
    
    protected:
    Canvas canvas;
    DisplayableSpace space;
};

#endif //PROJECTOR_H
