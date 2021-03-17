#include "face.h"

// NOTE(Alex): XY (0,0) (5,5) Z = 0
Color<int> Quad_XY_Z0::get_color(float u, float v)
{
    float tx = u;
    float ty = v;
    
    Vec3<float> c0 = c[0];
    Vec3<float> c1 = c[1];
    Vec3<float> c2 = c[2];
    Vec3<float> c3 = c[3];
    
    Vec3<float> a = Lerp(c0,c1, tx);
    Vec3<float> b = Lerp(c2,c3, tx);
    Vec3<float> d = Lerp(a,b,ty);
    
    int ir = int(255.99 * d.r);
    int ig = int(255.99 * d.g);
    int ib = int(255.99 * d.b);
    
    return {ir,ig,ib};
}


// NOTE(Alex): YZ (0,0) (5,5) X = 0
Color<int> Quad_YZ_X0::get_color(float u, float v)
{
    float tx = 1.0f - u;
    float ty = v;
    
    // NOTE(Alex): In this way the parameterization u,v is CORRECT!!!
    Vec3<float> c0 = c[1];
    Vec3<float> c1 = c[0];
    Vec3<float> c2 = c[3];
    Vec3<float> c3 = c[2];
    
    Vec3<float> a = Lerp(c0,c1, tx);
    Vec3<float> b = Lerp(c2,c3, tx);
    Vec3<float> d = Lerp(a,b,ty);
    
    int ir = int(255.99 * d.r);
    int ig = int(255.99 * d.g);
    int ib = int(255.99 * d.b);
    return {ir,ig,ib};
}


// NOTE(Alex): XZ (0,0) (5,5) Y = 0
Color<int> Quad_XZ_Y0::get_color(float u, float v)
{
    float tx = 1.0f - u;
    float ty = 1.0f - v;
    
    // NOTE(Alex): In this way the parameterization u,v is CORRECT!!!
    Vec3<float> c0 = c[3];
    Vec3<float> c1 = c[1];
    Vec3<float> c2 = c[2];
    Vec3<float> c3 = c[0];
    
    Vec3<float> a = Lerp(c0,c1, tx);
    Vec3<float> b = Lerp(c2,c3, tx);
    Vec3<float> d = Lerp(a,b,ty);
    
    int ir = int(255.99 * d.r);
    int ig = int(255.99 * d.g);
    int ib = int(255.99 * d.b);
    return {ir,ig,ib};
}

// NOTE(Alex): YZ (0,0) (5,5) X = 5.0f
Color<int> Quad_YZ_X5::get_color(float u, float v)
{
    float tx = u;
    float ty = v;
    
    // NOTE(Alex): In this way the parameterization u,v is CORRECT!!!
    Vec3<float> c0 = c[0];
    Vec3<float> c1 = c[1];
    Vec3<float> c2 = c[2];
    Vec3<float> c3 = c[3];
    
    Vec3<float> a = Lerp(c0,c1, tx);
    Vec3<float> b = Lerp(c2,c3, tx);
    Vec3<float> d = Lerp(a,b,ty);
    
    int ir = int(255.99 * d.r);
    int ig = int(255.99 * d.g);
    int ib = int(255.99 * d.b);
    return {ir,ig,ib};
}

// NOTE(Alex): XZ (0,0) (5,5) Y = 5.0f
Color<int> Quad_XZ_Y5::get_color(float u, float v)
{
    float tx = u;
    float ty = 1.0f - v;
    
    // NOTE(Alex): In this way the parameterization u,v is CORRECT!!!
    Vec3<float> c0 = c[1];
    Vec3<float> c1 = c[3];
    Vec3<float> c2 = c[0];
    Vec3<float> c3 = c[2];
    
    Vec3<float> a = Lerp(c0,c1, tx);
    Vec3<float> b = Lerp(c2,c3, tx);
    Vec3<float> d = Lerp(a,b,ty);
    
    int ir = int(255.99 * d.r);
    int ig = int(255.99 * d.g);
    int ib = int(255.99 * d.b);
    return {ir,ig,ib};
}
