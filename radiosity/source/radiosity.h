/* date = March 20th 2021 4:49 pm */

/* 
class Radiosity
referenced by: class Space
 Radiosity Solver
*/


#ifndef RADIOSITY_H
#define RADIOSITY_H

#include <string>
#include "matrix.h"
#include "ray.h"
#include "quad_manager.h"
#include "stimuli.h"

class Radiosity{
    public:
    Radiosity(float fw, int hps);
    Color<int> get_color(Ray ray, float tMin, float tMax){return qm.get_color(ray, tMin, tMax);}
    private:
    Quad_manager qm;
    Matrix<float,2> f;
    Stimuli r_s;
    Stimuli g_s;
    Stimuli b_s;
};

#endif //RADIOSITY_H
