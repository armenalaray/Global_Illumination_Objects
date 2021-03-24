#include "quad_manager.h"

Quad_manager::Quad_manager(float fw_, int hps_):
fw{fw_},
hps{hps_},
ei{0},
f_xy_z0{fw,hps,ei},
f_yz_x0{fw,hps,ei},
f_xz_y0{fw,hps,ei},
f_yz_x5{fw,hps,ei},
f_xz_y5{fw,hps,ei}
{
    /* 
    TODO(Alex): Emissor
     */
#if 0    
    // NOTE(Alex): Emissor
    {
        float half_d = fw * 0.25f;
        
        float x0 = fw * 0.5f - half_d;
        float x1 = fw * 0.5f + half_d;
        float z0 = fw * 0.5f - half_d;
        float z1 = fw * 0.5f + half_d;
        float y=fw - 0.1f;
        Vec3<float> p{x0+0.5f*x1, y,z0+0.5f*z1};
        quads.push_back(std::make_shared<Quad_XZ_Y5>(p, ei, x0,x1,z0,z1,y));
    }
#endif
    
    for(auto i:f_xy_z0.cm)
    {
        quads.push_back(i.first);
    }
    
    for(auto i:f_yz_x0.cm)
    {
        quads.push_back(i.first);
    }
    
    for(auto i:f_xz_y0.cm)
    {
        quads.push_back(i.first);
    }
    
    for(auto i:f_yz_x5.cm)
    {
        quads.push_back(i.first);
    }
    
    for(auto i:f_xz_y5.cm)
    {
        quads.push_back(i.first);
    }
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

void Quad_manager::move_radiosities(const Matrix<float,1>& r,const Matrix<float,1>& g,const Matrix<float,1>& b){
    f_xy_z0.add_radiosities(r,g,b);
    f_yz_x0.add_radiosities(r,g,b);
    f_xz_y0.add_radiosities(r,g,b);
    f_yz_x5.add_radiosities(r,g,b);
    f_xz_y5.add_radiosities(r,g,b);
}