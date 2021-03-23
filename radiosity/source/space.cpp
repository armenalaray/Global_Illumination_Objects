#include "space.h"

Space::Space():
qm{}
{
}

Color<int> Space::request_color(Ray r, float tMin, float tMax)
{
    return qm.get_color(r, tMin, tMax);
}

Matrix<float,2> Space::request_ff_matrix(){
    return qm.calc_ff();
}
