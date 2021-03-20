/* date = March 16th 2021 6:31 pm */

#ifndef VEC3_H
#define VEC3_H

#include <cmath>

/* 
Esta cosa no genera expecializacion Vec3<float> por que no lo necesita
Los templates solo funcionan en tiempo de compilacion, por eso no puedes compilarlo por separado!!!
 */
template<typename T>
class Vec3
{
    /* 
    // TODO(Alex): Add move constructors and destructors
     */
    public:
	Vec3(T e0, T e1, T e2):
    x{e0},y{e1},z{e2}{}
    Vec3& operator+=(const Vec3&);
    Vec3& operator-=(const Vec3&);
    
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
Vec3<T>& Vec3<T>::operator+=(const Vec3& v){
    x+=v.x;
    y+=v.y;
    z+=v.z;
    return *this;
}

template<typename T>
Vec3<T>& Vec3<T>::operator-=(const Vec3& v){
    x-=v.x;
    y-=v.y;
    z-=v.z;
    return *this;
}

template<typename T>
Vec3<T> operator+(const Vec3<T>& v1, const Vec3<T>& v2){
    Vec3<T> res{v1};
    res+=v2;
    return res;
}

template<typename T>
Vec3<T> operator-(const Vec3<T>& v1, const Vec3<T>& v2){
    Vec3<T> res{v1};
    res-=v2;
    return res;
}

template<typename T>
Vec3<T> operator*(const Vec3<T>& v, const float f){
    return {v.x*f, v.y*f, v.z*f};
}

template<typename T>
Vec3<T> operator*(const float f, const Vec3<T>& v){
    return v*f;
}

template<typename T>
using Color = Vec3<T>;

#endif //VEC3_H
