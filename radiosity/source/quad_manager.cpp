#include "quad_manager.h"

Quad_manager::Quad_manager(float fw_, int hps_):
fw{fw_},
hps{hps_},
quads{}
{
    float s = fw / float(hps);
    ElemIndex ei=0;
    
    // NOTE(Alex): XY (0,0) (5,5) Z = 0
    
    {
        float z = 0.0f;
        float y0 = 0;
        float y1 = y0 + s;
        for (size_t i = 0; i < hps ; i += 1, y0 += s, y1 += s){
            float x0 = 0; 
            float x1 = x0 + s; 
            for (size_t j = 0; j < hps; j += 1, x0 += s, x1 += s, ei += 1){
                Vec3<float> p{x0+0.5f*x1,y0+0.5f*y1,z};
                quads.push_back(std::make_shared<Quad_XY_Z0>(p, ei, x0,x1,y0,y1,z));
            }
        }
    }
    
    
    // NOTE(Alex): YZ (0,0) (5,5) X = 0
    
    {
        float x = 0.0f;
        float y0 = 0;
        float y1 = y0 + s;
        for (size_t i = 0; i < hps ; i += 1, y0 += s, y1 += s){
            float z0 = 0; 
            float z1 = z0 + s; 
            for (size_t j = 0; j < hps; j += 1, z0 += s, z1 += s, ei += 1){
                Vec3<float> p{x, y0+0.5f*y1,z0+0.5f*z1};
                quads.push_back(std::make_shared<Quad_YZ_X0>(p, ei, y0,y1,z0,z1,x));
            }
        }
    }
    
    // NOTE(Alex): XZ (0,0) (5,5) Y = 0
    
    {
        float y = 0.0f;
        float z0 = 0;
        float z1 = z0 + s;
        for (size_t i = 0; i < hps ; i += 1, z0 += s, z1 += s){
            float x0 = 0; 
            float x1 = x0 + s; 
            for (size_t j = 0; j < hps; j += 1, x0 += s, x1 += s, ei += 1){
                Vec3<float> p{x0+0.5f*x1, y,z0+0.5f*z1};
                quads.push_back(std::make_shared<Quad_XZ_Y0>(p, ei, x0,x1,z0,z1,y));
            }
        }
    }
    
    // NOTE(Alex): YZ (0,0) (5,5) X = 5.0f
    
    {
        float x = fw;
        float y0 = 0;
        float y1 = y0 + s;
        for (size_t i = 0; i < hps ; i += 1, y0 += s, y1 += s){
            float z0 = 0; 
            float z1 = z0 + s; 
            for (size_t j = 0; j < hps; j += 1, z0 += s, z1 += s, ei += 1){
                Vec3<float> p{x, y0+0.5f*y1,z0+0.5f*z1};
                quads.push_back(std::make_shared<Quad_YZ_X5>(p, ei, y0,y1,z0,z1,x));
            }
        }
    }
    
    // NOTE(Alex): XZ (0,0) (5,5) Y = 5.0f
    
    {
        float y = fw;
        float z0 = 0;
        float z1 = z0 + s;
        for (size_t i = 0; i < hps ; i += 1, z0 += s, z1 += s){
            float x0 = 0; 
            float x1 = x0 + s; 
            for (size_t j = 0; j < hps; j += 1, x0 += s, x1 += s, ei += 1){
                Vec3<float> p{x0+0.5f*x1, y,z0+0.5f*z1};
                quads.push_back(std::make_shared<Quad_XZ_Y5>(p, ei, x0,x1,z0,z1,y));
            }
        }
    }
    
    
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
