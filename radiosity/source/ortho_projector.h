/* date = March 16th 2021 2:42 pm */

/* 
class OrthoProjector
Inherits From: class Projector
Perspective Projector - Orthogonal renderer.
NOTE: This is a simple Rendering system just to showcase the Radiosity design to 
support multiple rendering systems.
 */

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
    void render(float fw, float fh, int th, int tw);
    private:
};

#endif //ORTHO_PROJECTOR_H
