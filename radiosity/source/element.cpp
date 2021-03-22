#include "element.h"

Element::Element():
hm{100,100}, data{}
{
    data.v = MakeUnitVector(data.n);
    data.u = MakeUnitVector(Cross(hm.vup,data.v));
    
    if(data.u.norm() == 0)
    {
        // NOTE(Alex): We need to test against other arbitrary vector that is not colinear to v
        data.u = MakeUnitVector(Cross(hm.other_vup,data.v));
    }
    
    data.w = Cross(data.v,data.u);
    
    data.corners[static_cast<int>(corner_it::tf)] = data.p +  data.v      + -(1.0f *data.u + 1.0f*data.w);  
    data.corners[static_cast<int>(corner_it::rf)] = data.p +  1.0f*data.u + -1.0f*data.w;  
    data.corners[static_cast<int>(corner_it::lf)] = data.p + -1.0f*data.u +  1.0f*data.w;  
    data.corners[static_cast<int>(corner_it::ff)] = data.p +  1.0f*data.u +  1.0f*data.w;  
    data.corners[static_cast<int>(corner_it::bf)] = data.p + -1.0f*data.u + -1.0f*data.w;
}

bool Element::get_ray(Ray& r){
    if(data.corner_i>=5) return false;
    switch(data.corner_i)
    {
        case corner_it::tf:
        {
            if(hm.y>=hm.yc) {++data.corner_i; hm.y=0; hm.x=0;}
            if(hm.x>=hm.xc) {hm.x=0;++hm.y;}
            
            float NX = 2.0f*(float(hm.x) / float(hm.xc)) + hm.half_pw;
            float NY = 2.0f*(float(hm.y) / float(hm.yc)) + hm.half_ph;
            
            Vec3<float> RDirection = (data.corners[data.corner_i] + NX*data.u + NY*data.w) - data.p;
            r = Ray{data.p, RDirection};
            
            ++hm.x;
        }break;
        case corner_it::rf:
        {
            if(hm.y>=hm.y_halfc) {++data.corner_i; hm.y=0; hm.x=0;}
            if(hm.x>=hm.xc){hm.x=0;++hm.y;}
            
            float NX = 2.0f*(float(hm.x) / float(hm.xc)) + hm.half_pw;
            float NY = 2.0f*(float(hm.y) / float(hm.yc)) + hm.half_ph;
            
            Vec3<float> RDirection = (data.corners[data.corner_i] + NX*data.w + NY*data.v) - data.p;
            r = Ray{data.p, RDirection};
            
            ++hm.x;
        }break;
        case corner_it::lf:
        {
            if(hm.y>=hm.y_halfc) {++data.corner_i; hm.y=0; hm.x=0;}
            if(hm.x>=hm.xc){hm.x=0;++hm.y;}
            
            float NX = 2.0f*(float(hm.x) / float(hm.xc)) + hm.half_pw;
            float NY = 2.0f*(float(hm.y) / float(hm.yc)) + hm.half_ph;
            
            Vec3<float> RDirection = (data.corners[data.corner_i] + -NX*data.w + NY*data.v) - data.p;
            r = Ray{data.p, RDirection};
            
            ++hm.x;
        }break;
        case corner_it::ff:
        {
            if(hm.y>=hm.y_halfc) {++data.corner_i; hm.y=0; hm.x=0;}
            if(hm.x>=hm.xc){hm.x=0;++hm.y;}
            
            float NX = 2.0f*(float(hm.x) / float(hm.xc)) + hm.half_pw;
            float NY = 2.0f*(float(hm.y) / float(hm.yc)) + hm.half_ph;
            
            Vec3<float> RDirection = (data.corners[data.corner_i] + -NX*data.u + NY*data.v) - data.p;
            r = Ray{data.p, RDirection};
            
            ++hm.x;
        }break;
        case corner_it::bf:
        {
            if(hm.y>=hm.y_halfc) {++data.corner_i; hm.y=0; hm.x=0;}
            if(hm.x>=hm.xc){hm.x=0;++hm.y;}
            
            float NX = 2.0f*(float(hm.x) / float(hm.xc)) + hm.half_pw;
            float NY = 2.0f*(float(hm.y) / float(hm.yc)) + hm.half_ph;
            
            Vec3<float> RDirection = (data.corners[data.corner_i] + NX*data.u + NY*data.v) - data.p;
            r = Ray{data.p, RDirection};
            
            ++hm.x;
        }break;
    }
    return true;
}


void Element::calc_ff(const Ray& ray, const Element& j, Matrix_2d<float>& ffm){
    float r = ray.get_direction().squared_norm();
    Vec3<float> ij = MakeUnitVector(ray.get_direction());
    Vec3<float> ji = MakeUnitVector(-ray.get_direction());
    float Dotji = dot(j.data.n,ji);
    
    if(Dotji > 0.0f)
    {
        float Value{((dot(data.v, ij) * Dotji * hm.da) / ((float)M_PI  * r * r))};
        ffm(data.i,j.data.i) += Value;
    }
}

