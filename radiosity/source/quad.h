/* date = March 16th 2021 2:53 pm */

/* 
class Quad
Inherits From: class hitable, class Element
Derived Classes: class Quad_XY_Z0, class Quad_YZ_X0, class Quad_XZ_Y0, class Quad_YZ_X5, class Quad_XZ_Y5
Base Class that represents the interface for each Quad, it has to inherit Element properties and hitable properties.
 */

/* 
struct HitRec
 Basic Structure to return data from hit procedure.
*/

/* 
class hitable
Derived Classes: class Quad
Represents interface for hitable objects.
*/

#ifndef QUAD_H
#define QUAD_H

#include "utility.h"
#include "ray.h"
#include "element.h"


struct HitRec{
    float u;
    float v;
    float t;
};

class hitable
{
    public:
    hitable(){};
    virtual bool hit(Ray & r, float tMin, float tMax, HitRec & HitRecord) = 0;
};

class Quad : public hitable, public Element
{
    public:
    Quad(const Vec3<float>& n, const Vec3<float>& p, const ElemIndex i,
         const Color<float>& c0 = Color<float>{},
         const Color<float>& c1 = Color<float>{},
         const Color<float>& c2 = Color<float>{},
         const Color<float>& c3 = Color<float>{}):
    hitable{},
    Element{n,p,i},
    c{{c0},{c1},{c2},{c3}}
    {}
    virtual bool hit(Ray & r, float tMin, float tMax, HitRec & HitRecord) = 0;
    virtual Color<int> get_color(float u, float v) = 0;
    Color<float> c[4];
};


class Quad_XY_Z0 : public Quad 
{
    public:
    Quad_XY_Z0(const Vec3<float>& p, const ElemIndex i,
               const float x0_,const float x1_,const float y0_,const float y1_,const float k_):
    Quad{Vec3<float>{0,0,1.f},p,i},
    x0{x0_},x1{x1_},y0{y0_},y1{y1_},k{k_}
    {}
    // NOTE(Alex): hitable override
    bool hit(Ray & r, float tMin, float tMax, HitRec & HitRecord) override;
    // NOTE(Alex): Quad override
    Color<int> get_color(float u, float v) override;
    
    private:
    float x0,x1,y0,y1,k;
};

class Quad_YZ_X0 : public Quad
{
    public:
    Quad_YZ_X0(const Vec3<float>& p, const ElemIndex i,
               const float y0_,const float y1_,const float z0_,const float z1_,const float k_):
    Quad{Vec3<float>{1.f,0,0},p,i},
    y0{y0_},y1{y1_},z0{z0_},z1{z1_},k{k_}
    {}
    
    // NOTE(Alex): hitable override
    bool hit(Ray & r, float tMin, float tMax, HitRec & HitRecord) override;
    // NOTE(Alex): Quad override
    Color<int> get_color(float u, float v) override;
    private:
    float y0,y1,z0,z1,k;
};

class Quad_XZ_Y0 : public Quad
{
    public:
    Quad_XZ_Y0(const Vec3<float>& p, const ElemIndex i,
               const float x0_,const float x1_,const float z0_,const float z1_,const float k_):
    Quad{Vec3<float>{0,1.f,0},p,i},
    x0{x0_},x1{x1_},z0{z0_},z1{z1_},k{k_}
    {}
    
    // NOTE(Alex): hitable override
    bool hit(Ray & r, float tMin, float tMax, HitRec & HitRecord) override;
    // NOTE(Alex): Quad override
    Color<int> get_color(float u, float v) override;
    private:
    float x0,x1,z0,z1,k;
};

class Quad_YZ_X5 : public Quad
{
    public:
    Quad_YZ_X5(const Vec3<float>& p, const ElemIndex i,
               const float y0_,const float y1_,const float z0_,const float z1_,const float k_):
    Quad{Vec3<float>{-1.f,0,0},p,i},
    y0{y0_},y1{y1_},z0{z0_},z1{z1_},k{k_}
    {}
    
    // NOTE(Alex): hitable override
    bool hit(Ray & r, float tMin, float tMax, HitRec & HitRecord) override;
    // NOTE(Alex): Quad override
    Color<int> get_color(float u, float v) override;
    private:
    float y0,y1,z0,z1,k;
};

class Quad_XZ_Y5 : public Quad
{
    public:
    Quad_XZ_Y5(const Vec3<float>& p, const ElemIndex i,
               const float x0_,const float x1_,const float z0_,const float z1_,const float k_):
    Quad{Vec3<float>{0,-1.f,0},p,i},
    x0{x0_},x1{x1_},z0{z0_},z1{z1_},k{k_}
    {}
    
    // NOTE(Alex): hitable override
    bool hit(Ray & r, float tMin, float tMax, HitRec & HitRecord) override;
    // NOTE(Alex): Quad override
    Color<int> get_color(float u, float v) override;
    private:
    float x0,x1,z0,z1,k;
};
#endif

