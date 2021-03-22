#include "quad_manager.h"

Color<int> Quad_manager::get_color(Ray r, float tMin, float tMax){
    HitRec rec{};
    std::shared_ptr<Quad> a_ref{};
    for(auto a:quads)
    {
        if(a->hit(r, tMin, tMax, rec))
        {
            tMax = rec.t;
            a_ref=a;
        }
    }
    if(a_ref.get()) return a_ref->get_color(rec.u,rec.v);
    else return {0,0,0};
}

Matrix_2d<float> Quad_manager::calc_ff(){
    Matrix_2d<float> ff(quads.size(),quads.size());
    for(auto a:quads){
        Ray r{};
        while(a->get_ray(r))
        {
            Element j{};
            if(request_element(r, 0.001f, FLT_MAX, j))
                a->calc_ff(r, j, ff);
        }
    }
    return ff;
}

bool Quad_manager::request_element(Ray r, float tMin, float tMax, Element& e){
    HitRec rec{};
    std::shared_ptr<Quad> a_ref{};
    for(auto a:quads){
        if(a->hit(r, tMin, tMax, rec))
        {
            tMax = rec.t;
            a_ref = a;
        }
    }
    // NOTE(Alex): Slicing
    if(a_ref.get()){e = *a_ref; return true;}
    else return false;
}