/* date = March 20th 2021 2:29 pm */

#ifndef PERSP_PROJECTOR_H
#define PERSP_PROJECTOR_H

#include "vec3.h"
#include "ray.h"
#include "canvas.h"
#include "projector.h"
#include "space.h"

extern Space space;

class PerspectiveProjector : public Projector
{
    public:
    void render(float rfw, float rfh, int th, int tw);
    private:
};

#endif //PERSP_PROJECTOR_H
