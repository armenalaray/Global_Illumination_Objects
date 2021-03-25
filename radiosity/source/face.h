/* date = March 24th 2021 11:43 am */

#ifndef FACE_H
#define FACE_H

#include <memory>
#include <vector>
#include <string>

#include "matrix.h"
#include "quad.h"
#include "mapped_quad.h"

struct Mapping{
    Mapping(){};
    Mapping(const Mapped_quad_ur& ur_,
            const Mapped_quad_u& u_,
            const Mapped_quad_ul& ul_,
            const Mapped_quad_r& r_,
            const Mapped_quad_k& k_,
            const Mapped_quad_l& l_,
            const Mapped_quad_br& br_,
            const Mapped_quad_b& b_,
            const Mapped_quad_bl& bl_);
    Mapped_quad_ur ur;
    Mapped_quad_u u;
    Mapped_quad_ul ul;
    Mapped_quad_r r;
    Mapped_quad_k k; //k is self-pointer, it MUST be filled always!!!!
    Mapped_quad_l l;
    Mapped_quad_br br;
    Mapped_quad_b b;
    Mapped_quad_bl bl;
};

class Face{
    public:
    Face(ElemIndex ei, size_t i, size_t j):
    si{ei},
    f(i,j)
    {
        cm.reserve(i*j);
    }
    void add_radiosities(const Matrix<float,1>& r,const Matrix<float,1>& g,const Matrix<float,1>& b);
    void debug_print(std::string fn);
    protected:
    void generate_mapping();
    ElemIndex si;
    Matrix<std::shared_ptr<Quad>,2> f;
    std::vector<std::pair<std::shared_ptr<Quad>,Mapping>> cm;
};

class Face_xy_z0:public Face{
    public:
    Face_xy_z0(float fw, int hps, ElemIndex& ei, std::vector<std::shared_ptr<Quad>>& qmv);
};

class Face_yz_x0:public Face{
    public:
    Face_yz_x0(float fw, int hps, ElemIndex& ei, std::vector<std::shared_ptr<Quad>>& qmv);
};

class Face_xz_y0:public Face{
    public:
    Face_xz_y0(float fw, int hps, ElemIndex& ei, std::vector<std::shared_ptr<Quad>>& qmv);
};

class Face_yz_x5:public Face{
    public:
    Face_yz_x5(float fw, int hps, ElemIndex& ei, std::vector<std::shared_ptr<Quad>>& qmv);
};

class Face_xz_y5:public Face{
    public:
    Face_xz_y5(float fw, int hps, ElemIndex& ei, std::vector<std::shared_ptr<Quad>>& qmv);
};

class Face_emissor:public Face{
    public:
    Face_emissor(float fw, int hps, ElemIndex& ei, std::vector<std::shared_ptr<Quad>>& qmv);
};


#endif //FACE_H
