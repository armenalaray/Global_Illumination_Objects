#include "mapped_quad.h"

void Mapped_quad_ur::add_color(const Color<float>& c){
    if(qp.get())
    {
        qp->c[0]+=c;
    }
}

void Mapped_quad_u::add_color(const Color<float>& c){
    if(qp.get())
    {
        qp->c[0]+=c;
        qp->c[1]+=c;
    }
}

void Mapped_quad_ul::add_color(const Color<float>& c){
    if(qp.get())
    {
        qp->c[1]+=c;
    }
}

void Mapped_quad_r::add_color(const Color<float>& c){
    if(qp.get())
    {
        qp->c[0]+=c;
        qp->c[2]+=c;
    }
}

void Mapped_quad_k::add_color(const Color<float>& c){
    if(qp.get())
    {
        qp->c[0]+=c;
        qp->c[1]+=c;
        qp->c[2]+=c;
        qp->c[3]+=c;
    }
}

void Mapped_quad_l::add_color(const Color<float>& c){
    if(qp.get())
    {
        qp->c[1]+=c;
        qp->c[3]+=c;
    }
}

void Mapped_quad_br::add_color(const Color<float>& c){
    if(qp.get())
    {
        qp->c[2]+=c;
    }
}

void Mapped_quad_b::add_color(const Color<float>& c){
    if(qp.get())
    {
        qp->c[2]+=c;
        qp->c[3]+=c;
    }
}

void Mapped_quad_bl::add_color(const Color<float>& c){
    if(qp.get())
    {
        qp->c[3]+=c;
    }
}
