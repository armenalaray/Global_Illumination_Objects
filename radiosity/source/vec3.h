/* date = March 16th 2021 6:31 pm */

#ifndef VEC3_H
#define VEC3_H

#include <cmath>

template<typename T>
class Vec3
{
    public:
	Vec3(T e0, T e1, T e2):
    x{e0},y{e1},z{e2}{}
    
    Vec3& operator*(const float);
    
    private:
    union{
        struct{
            T x;
            T y;
            T z;
        };
        struct{
            T r;
            T g;
            T b;
        };
    };
};

template<typename T>
using Color = Vec3<T>;

#endif //VEC3_H
