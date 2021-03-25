/* date = March 21st 2021 9:57 am */

#ifndef QUAD_MANAGER_H
#define QUAD_MANAGER_H

#include <memory>
#include "vec3.h"
#include "ray.h"
#include "quad.h"
#include "element.h"
#include "face.h"

class Quad_manager{
    public:
    Quad_manager(float fw, int hps);
    Color<int> get_color(Ray r, float tMin, float tMax);
    Matrix<float,2> calc_ff();
    void move_radiosities(const Matrix<float,1>& r,const Matrix<float,1>& g,const Matrix<float,1>& b);
    private:
    bool request_element(Ray r, float tMin, float tMax, Element_ref& e);
    float fw;
    int hps;
    ElemIndex ei;
    std::vector<std::shared_ptr<Quad>> quads;
    Face_xy_z0 f_xy_z0;
    Face_yz_x0 f_yz_x0;
    Face_xz_y0 f_xz_y0;
    Face_yz_x5 f_yz_x5;
    Face_xz_y5 f_xz_y5;
    Face_emissor e;
};

#endif //QUAD_MANAGER_H
