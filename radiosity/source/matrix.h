/* date = March 20th 2021 2:04 pm */

#ifndef MATRIX_H
#define MATRIX_H

#include <vector>
#include <array>

template<size_t N>
class Matrix_desc{
    public:
    private:
    std::array<size_t,N> extents;
    std::array<size_t,N> strides;
};

template<typename T,size_t N>
class Matrix_ref{
    public:
    template<typename M>
        Matrix_ref& operator=(M& other);
    private:
    Matrix_desc<N> desc;
    T * elem;
};

template<typename T,size_t N>
template<typename M>
Matrix_ref<T,N>& Matrix_ref<T,N>::operator=(M& other){
    return *this;
}

template<typename T,size_t N>
class Matrix{
    public:
    Matrix_ref<T,N-1> row(size_t i);
    private:
    Matrix_desc<N> desc;
    std::vector<T> elem;
};

template<typename T, size_t N>
Matrix_ref<T,N-1> Matrix<T,N>::row(size_t i){
    Matrix_ref<T,N-1> res;
    return res;
}

#endif //MATRIX_H
