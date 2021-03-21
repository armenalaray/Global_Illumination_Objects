/* date = March 16th 2021 2:34 pm */

#ifndef PROJECTOR_H
#define PROJECTOR_H

#include "vec3.h"
#include "canvas.h"
#include "space.h"

static Space space;

class Projector{
    public:
    Projector()=default;
    virtual void render() = 0;
    protected:
    void CreateSpace();
};

#endif //PROJECTOR_H
