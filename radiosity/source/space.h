/* date = March 16th 2021 2:46 pm */

#ifndef SPACE_H
#define SPACE_H

/* 
Esta es mi interface, puede preguntar a sus quds si son colisionados por el ray y si si, les pregunta
el color por medio de los uv
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
