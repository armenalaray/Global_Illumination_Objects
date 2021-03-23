#include "quad_manager.h"

Quad_manager::Quad_manager()
{
    quads.push_back(std::make_shared<Quad_XY_Z0>(Vec3<float>{0,0,0}, 0, 0.f,5.f,0.f,5.f,0.f));
}

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

Matrix<float,2> Quad_manager::calc_ff(){
    Matrix<float,2> ff(quads.size(),quads.size());
    for(auto a:quads){
        Ray r{};
        while(a->get_ray(r))
        {
            Element_ref j{};
            if(request_element(r, 0.001f, FLT_MAX, j))
                a->calc_ff(r, j, ff);
        }
    }
    return ff;
}

bool Quad_manager::request_element(Ray r, float tMin, float tMax, Element_ref& e){
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