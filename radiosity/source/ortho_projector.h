/* date = March 16th 2021 2:42 pm */

#ifndef ORTHO_PROJECTOR_H
#define ORTHO_PROJECTOR_H

#include "vec3.h"
#include "projector.h"

class OrthoProjector : public Projector 
{
    public:
    void render() override;
    private:
};

#endif //ORTHO_PROJECTOR_H
