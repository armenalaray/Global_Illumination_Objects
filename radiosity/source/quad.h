/* date = March 16th 2021 2:53 pm */

#ifndef FACE_H
#define FACE_H

#include "utility.h"
#include "ray.h"

struct HitRec{
    float u;
    float v;
    float t;
};

class hitable
{
    public:
    virtual void Hit(Ray & r, float tMin, float tMax, HitRec & HitRecord) = 0;
};


class Quad : public hitable
{
    public:
    // NOTE(Alex): hitable override
    virtual void hit(Ray & r, float tMin, float tMax, HitRec & HitRecord);
    // NOTE(Alex): Quad override
    virtual Color<int> get_color(float u, float v);
    
    private:
    Vec3<float> c[4];
};

class Quad_XY_Z0 : public Quad
{
    public:
    // NOTE(Alex): hitable override
    void hit(Ray & r, float tMin, float tMax, HitRec & HitRecord) override;
    // NOTE(Alex): Quad override
    Color<int> get_color(float u, float v) override;
};

class Quad_YZ_X0 : public Quad
{
    public:
    // NOTE(Alex): hitable override
    void hit(Ray & r, float tMin, float tMax, HitRec & HitRecord) override;
    // NOTE(Alex): Quad override
    Color<int> get_color(float u, float v) override;
};

class Quad_XZ_Y0 : public Quad
{
    public:
    // NOTE(Alex): hitable override
    void hit(Ray & r, float tMin, float tMax, HitRec & HitRecord) override;
    // NOTE(Alex): Quad override
    Color<int> get_color(float u, float v) override;
};

class Quad_YZ_X5 : public Quad
{
    public:
    // NOTE(Alex): hitable override
    void hit(Ray & r, float tMin, float tMax, HitRec & HitRecord) override;
    // NOTE(Alex): Quad override
    Color<int> get_color(float u, float v) override;
};

class Quad_XZ_Y5 : public Quad
{
    public:
    // NOTE(Alex): hitable override
    void hit(Ray & r, float tMin, float tMax, HitRec & HitRecord) override;
    // NOTE(Alex): Quad override
    Color<int> get_color(float u, float v) override;
};

#endif //FACE_H
