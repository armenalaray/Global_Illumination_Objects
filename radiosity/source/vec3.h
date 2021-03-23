/* date = March 16th 2021 6:31 pm */

#ifndef VEC3_H
#define VEC3_H

#include <ostream>
#include <cmath>

/* 
Esta cosa no genera expecializacion Vec3<float> por que no lo necesita
Los templates solo funcionan en tiempo de compilacion, por eso no puedes compilarlo por separado!!!
 */
template<typename T>
class Vec3
{
    public:
	Vec3()=default;
    Vec3(T e0, T e1, T e2):
    x{e0},y{e1},z{e2}{}
    
    Vec3& operator+=(const Vec3&);
    Vec3& operator-=(const Vec3&);
    Vec3& operator*=(const float);
    
    T norm() const;
    T squared_norm() const;
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
std::ostream& operator<<(std::ostream& o, Vec3<T> v){
    return o<<'{' << v.x << v.y << v.z << '}';
}

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
Vec3<T>& Vec3<T>::operator*=(const float f){
    x*=f;
    y*=f;
    z*=f;
    return *this;
}

template<typename T>
T Vec3<T>::squared_norm()const{
    return x*x+y*y+z*z;
}

template<typename T>
T dot(const Vec3<T> v1, const Vec3<T> v2){
    return v1.x*v2.x+v1.y*v2.y+v1.z*v2.z;
}


template<typename T>
T Vec3<T>::norm()const{
    return std::sqrt(squared_norm());
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
Vec3<T> operator-(const Vec3<T> v){
    return {-v.x,-v.y,-v.z};
}

template<typename T>
Vec3<T> MakeUnitVector(const Vec3<T> v){
    Vec3<T> res{};
    if(v.norm() != 0)
    {
        res=v;
        T k = T{1.0f} / v.norm();
        res*=k;
    }
    return res;
}

template<typename T>
Vec3<T> Cross(const Vec3<T> v1,const Vec3<T> v2){
    return {
        v1.y * v2.z - v1.z * v2.y, 
        -(v1.x * v2.z - v1.z * v2.x),   
        v1.x * v2.y - v1.y * v2.x
    };
}



template<typename T>
using Color = Vec3<T>;

#endif //VEC3_H
