#include "space.h"

Space::Space(float fw, int hps):
r{fw,hps}
{
}

Color<int> Space::request_color(Ray ray, float tMin, float tMax)
{
    return r.get_color(ray, tMin, tMax);
}
