/* date = March 21st 2021 9:57 am */

#ifndef QUAD_MANAGER_H
#define QUAD_MANAGER_H

#include <memory>
#include "vec3.h"
#include "ray.h"
#include "quad.h"
#include "element.h"
class Quad_manager{
    public:
    Quad_manager(float fw, int hps);
    Color<int> get_color(Ray r, float tMin, float tMax);
    Matrix<float,2> calc_ff();
    int get_face_count();
    int get_hitables_per_side_face();
    private:
    bool request_element(Ray r, float tMin, float tMax, Element_ref& e);
    float fw;
    int hps;
    std::vector<std::shared_ptr<Quad>> quads;
};

#endif //QUAD_MANAGER_H
