#include "element.h"

Element_impl::Element_impl(const Vec3<float>& n, const Vec3<float>& p):
u{},
v{},
w{},
corner_i{0},
corners{},
hm{100,100}
{
    v = MakeUnitVector(n);
    u = MakeUnitVector(Cross(hm.vup,v));
    
    if(u.norm() == 0)
    {
        // NOTE(Alex): We need to test against other arbitrary vector that is not colinear to v
        u = MakeUnitVector(Cross(hm.other_vup,v));
    }
    
    w = Cross(v,u);
    
    corners[static_cast<int>(corner_it::tf)] = p +  v      + -(1.0f *u + 1.0f*w);  
    corners[static_cast<int>(corner_it::rf)] = p +  1.0f*u + -1.0f*w;  
    corners[static_cast<int>(corner_it::lf)] = p + -1.0f*u +  1.0f*w;  
    corners[static_cast<int>(corner_it::ff)] = p +  1.0f*u +  1.0f*w;  
    corners[static_cast<int>(corner_it::bf)] = p + -1.0f*u + -1.0f*w;
}

// TODO(Alex): Implement other constructors and assignment operators?
Element_ref& Element_ref::operator=(const Element& e){
    n = e.get_n();
    p = e.get_p();
    i = e.get_i();
    return *this;
}

Element::Element(const Vec3<float>& n_, const Vec3<float>& p_, const ElemIndex i_):
n{n_},
p{p_},
i{i_},
impl{n,p}
{
}

bool Element::get_ray(Ray& r){
    if(impl.corner_i>=5) return false;
    switch(impl.corner_i)
    {
        case corner_it::tf:
        {
            if(impl.hm.y>=impl.hm.yc) {++impl.corner_i; impl.hm.y=0; impl.hm.x=0;}
            if(impl.hm.x>=impl.hm.xc) {impl.hm.x=0;++impl.hm.y;}
            
            float NX = 2.0f*(float(impl.hm.x) / float(impl.hm.xc)) + impl.hm.half_pw;
            float NY = 2.0f*(float(impl.hm.y) / float(impl.hm.yc)) + impl.hm.half_ph;
            
            Vec3<float> RDirection = (impl.corners[impl.corner_i] + NX*impl.u + NY*impl.w) - p;
            r = Ray{p, RDirection};
            
            ++impl.hm.x;
        }break;
        case corner_it::rf:
        {
            if(impl.hm.y>=impl.hm.y_halfc) {++impl.corner_i; impl.hm.y=0; impl.hm.x=0;}
            if(impl.hm.x>=impl.hm.xc){impl.hm.x=0;++impl.hm.y;}
            
            float NX = 2.0f*(float(impl.hm.x) / float(impl.hm.xc)) + impl.hm.half_pw;
            float NY = 2.0f*(float(impl.hm.y) / float(impl.hm.yc)) + impl.hm.half_ph;
            
            Vec3<float> RDirection = (impl.corners[impl.corner_i] + NX*impl.w + NY*impl.v) - p;
            r = Ray{p, RDirection};
            
            ++impl.hm.x;
        }break;
        case corner_it::lf:
        {
            if(impl.hm.y>=impl.hm.y_halfc) {++impl.corner_i; impl.hm.y=0; impl.hm.x=0;}
            if(impl.hm.x>=impl.hm.xc){impl.hm.x=0;++impl.hm.y;}
            
            float NX = 2.0f*(float(impl.hm.x) / float(impl.hm.xc)) + impl.hm.half_pw;
            float NY = 2.0f*(float(impl.hm.y) / float(impl.hm.yc)) + impl.hm.half_ph;
            
            Vec3<float> RDirection = (impl.corners[impl.corner_i] + -NX*impl.w + NY*impl.v) - p;
            r = Ray{p, RDirection};
            
            ++impl.hm.x;
        }break;
        case corner_it::ff:
        {
            if(impl.hm.y>=impl.hm.y_halfc) {++impl.corner_i; impl.hm.y=0; impl.hm.x=0;}
            if(impl.hm.x>=impl.hm.xc){impl.hm.x=0;++impl.hm.y;}
            
            float NX = 2.0f*(float(impl.hm.x) / float(impl.hm.xc)) + impl.hm.half_pw;
            float NY = 2.0f*(float(impl.hm.y) / float(impl.hm.yc)) + impl.hm.half_ph;
            
            Vec3<float> RDirection = (impl.corners[impl.corner_i] + -NX*impl.u + NY*impl.v) - p;
            r = Ray{p, RDirection};
            
            ++impl.hm.x;
        }break;
        case corner_it::bf:
        {
            if(impl.hm.y>=impl.hm.y_halfc) {++impl.corner_i; impl.hm.y=0; impl.hm.x=0;}
            if(impl.hm.x>=impl.hm.xc){impl.hm.x=0;++impl.hm.y;}
            
            float NX = 2.0f*(float(impl.hm.x) / float(impl.hm.xc)) + impl.hm.half_pw;
            float NY = 2.0f*(float(impl.hm.y) / float(impl.hm.yc)) + impl.hm.half_ph;
            
            Vec3<float> RDirection = (impl.corners[impl.corner_i] + NX*impl.u + NY*impl.v) - p;
            r = Ray{p, RDirection};
            
            ++impl.hm.x;
        }break;
    }
    return true;
}


void Element::calc_ff(const Ray& ray, const Element_ref& j, Matrix<float,2>& ffm){
    float r = ray.get_direction().squared_norm();
    Vec3<float> ij = MakeUnitVector(ray.get_direction());
    Vec3<float> ji = MakeUnitVector(-ray.get_direction());
    float Dotji = dot(j.n,ji);
    
    if(Dotji > 0.0f)
    {
        float Value{((dot(impl.v, ij) * Dotji * impl.hm.da) / ((float)M_PI  * r * r))};
        ffm(i,j.i) += Value;
    }
}

