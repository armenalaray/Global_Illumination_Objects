/* date = March 16th 2021 2:34 pm */

#ifndef PROJECTOR_H
#define PROJECTOR_H

#include "vec3.h"
#include "canvas.h"
#include "space.h"

class Projector{
    public:
    Projector():
    space{}
    {}
    
    virtual void render() = 0;
    
    protected:
    DisplayableSpace space;
};

#endif //PROJECTOR_H
