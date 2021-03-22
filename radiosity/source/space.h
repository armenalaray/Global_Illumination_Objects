/* date = March 16th 2021 2:46 pm */

#ifndef SPACE_H
#define SPACE_H

/* 
Esta es mi interface, puede preguntar a sus quds si son colisionados por el ray y si si, les pregunta
el color por medio de los uv
 */

#include <vector>
#include <memory>

#include "radiosity_solver.h"
#include "displayable.h"
#include "ray.h"
#include "quad_manager.h"

class Space : public Displayable, public Radiosity_solver
{
    public:
    Space()=default;
    // NOTE(Alex): Displayable override
    Color<int> request_color(Ray r, float tMin, float tMax) override;
    
    // NOTE(Alex): Integrator override
    Matrix_2d<float> request_ff_matrix() override;
    private:
    Quad_manager qm;
};


#endif //SPACE_H
