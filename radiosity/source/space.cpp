#include "space.h"

Color<int> Space::request_color(Ray r, float tMin, float tMax)
{
    return qm.get_color(r, tMin, tMax);
}

Matrix_2d<float> Space::request_ff_matrix(){
    return qm.calc_ff();
}
