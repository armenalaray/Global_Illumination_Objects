/* date = March 16th 2021 2:42 pm */

#ifndef ORTHO_PROJECTOR_H
#define ORTHO_PROJECTOR_H

#include "vec3.h"
#include "ray.h"
#include "canvas.h"
#include "projector.h"
#include "space.h"

extern Space space;

class OrthoProjector : public Projector 
{
    public:
    void render() override;
    private:
};

#endif //ORTHO_PROJECTOR_H
