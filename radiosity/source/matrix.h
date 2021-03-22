/* date = March 20th 2021 2:04 pm */

#ifndef MATRIX_H
#define MATRIX_H

#include <vector>
#include <array>
#include <initializer_list>

template<typename T>
class Matrix_2d;
/* 
I want the data as Rows!!!
 */
class Matrix_desc_2d{
    public:
    Matrix_desc_2d():extents{},strides{}{}
    Matrix_desc_2d(const size_t i, const size_t j);
    size_t operator()(const size_t row_i, const size_t col_i);
    size_t size()const{return extents[0]*extents[1];}
    private:
    std::array<size_t,2> extents;
    std::array<size_t,2> strides;
};

/* 
template<typename T>
class Matrix_ref_2d{
    public:
    Matrix_ref_2d& operator+=(const float);
    private:
    Matrix_2d<T> * p;
};

template<typename T>
Matrix_ref_2d<T>& Matrix_ref_2d<T>::operator+=(const float f){
    
    return *this;
}
 */

template<typename T>
class Matrix_2d{
    public:
    template<typename U>
        Matrix_2d(std::initializer_list<U>)=delete;
    template<typename U>
        Matrix_2d& operator=(std::initializer_list<U>)=delete;
    
    Matrix_2d():desc{},elem{}{}
    Matrix_2d(const size_t i, const size_t j):
    desc{i,j}
    {
        elem.reserve(desc.size());
    }
    T& operator()(const size_t row_i, const size_t col_i);
    private:
    Matrix_desc_2d desc;
    std::vector<T> elem;
};


template<typename T>
T& Matrix_2d<T>::operator()(const size_t row_i, const size_t col_i){
    return elem[desc(row_i,col_i)];
}

#endif //MATRIX_H
