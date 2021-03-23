/* date = March 20th 2021 2:04 pm */

#ifndef MATRIX_H
#define MATRIX_H

#include <vector>
#include <array>
#include <initializer_list>
#include <string>

template<typename T>
class Matrix_2d;

/* 
I want the data as Rows!!!
 */

/* 
Matrix_desc
N>2 not defined
 */
template<size_t N>
class Matrix_desc;

template<>
class Matrix_desc<2>{
    public:
    Matrix_desc():extents{},strides{}{}
    Matrix_desc(const size_t rc, const size_t cc):
    extents{rc,cc},
    strides{rc,0}
    {}
    size_t operator()(const size_t row, const size_t col){
        return strides[0]*row+strides[1]*col;
    }
    const size_t size()const{return extents[0]*extents[1];}
    const size_t extent(size_t i)const{if(i<=1)return extents[i];return 0;}
    private:
    std::array<size_t,2> extents;
    std::array<size_t,2> strides;
};

template<>
class Matrix_desc<1>{
    public:
    Matrix_desc():extents{},strides{}{}
    Matrix_desc(const size_t n):
    extents{n},
    strides{0}
    {}
    size_t operator()(const size_t n){
        return strides[0]*n;
    }
    const size_t size()const{return extents[0];}
    const size_t extent()const{return extents[0];}
    private:
    std::array<size_t,1> extents;
    std::array<size_t,1> strides;
};


/* 
Primary Template
// NOTE(Alex): N>2 not defined
 */
template<typename T, size_t N>
class Matrix;

/* 
// NOTE(Alex): Matrix Specialization
 */
template<typename T>
class Matrix<T,2>{
    public:
    template<typename U>
        Matrix(std::initializer_list<U>)=delete;
    template<typename U>
        Matrix& operator=(std::initializer_list<U>)=delete;
    
    Matrix():desc{},elem{}{}
    Matrix(const size_t i, const size_t j):
    desc{i,j}
    {
        elem.reserve(desc.size());
    }
    T& operator()(const size_t row_i, const size_t col_i);
    size_t get_extent(size_t i){return desc.extent(i);}
    Matrix<T,2>& make_diagonal(const Matrix<T,1> & o);
    void debug_print(std::string);
    private:
    Matrix_desc<2> desc;
    std::vector<T> elem;
};

template<typename T>
Matrix<T,2>& Matrix<T,2>::make_diagonal(const Matrix<T,1>& o){
    return *this;
}
#if 0
void MakeDiagonal()
{
    for (size_t i = 0; i < RowCount; i += 1){
        for (size_t j = 0; j < ColumnCount; j += 1){
            if(i == j)
            {
                Data[i][j] = T(1.0f);
            }
        }
    }
}
#endif


template<typename T>
T& Matrix<T,2>::operator()(const size_t row_i, const size_t col_i){
    return elem[desc(row_i,col_i)];
}

template<typename T>
Matrix<T,2> mult_m(const Matrix<T,2>& m1,const Matrix<T,2>& m2){
    Matrix<T,2> res{};
    return res;
}

template<typename T>
Matrix<T,2> sub_m(const Matrix<T,2>& m1,const Matrix<T,2>& m2){
    Matrix<T,2> res{};
    return res;
}

template<typename T>
void num_solver_gs(const Matrix<T,2>& k, Matrix<T,1>& b,const Matrix<T,1>& e){
    
}


template<typename T>
void Matrix<T,2>::debug_print(std::string fn){
    
}

/* 
// NOTE(Alex): Vector Specialization
 */
template<typename T>
class Matrix<T,1>{
    public:
    template<typename U>
        Matrix(std::initializer_list<U>)=delete;
    template<typename U>
        Matrix& operator=(std::initializer_list<U>)=delete;
    
    Matrix():desc{},elem{}{}
    Matrix(const size_t i):
    desc{i}
    {
        elem.reserve(desc.size());
    }
    T& operator()(const size_t n);
    size_t get_extent(){return desc.extent();}
    void debug_print(std::string);
    T squared_norm();
    private:
    Matrix_desc<1> desc;
    std::vector<T> elem;
};

template<typename T>
T Matrix<T,1>::squared_norm(){
    return T{};
}

template<typename T>
T& Matrix<T,1>::operator()(const size_t n){
    return elem[desc(n)];
}

template<typename T>
Matrix<T,1> mult_m(const Matrix<T,2>& m1,const Matrix<T,1>& m2){
    Matrix<T,1> res{};
    return res;
}

template<typename T>
Matrix<T,1> sub_m(const Matrix<T,1>& m1,const Matrix<T,1>& m2){
    Matrix<T,1> res{};
    return res;
}

template<typename T>
void Matrix<T,1>::debug_print(std::string fn){
    
}


#endif //MATRIX_H
