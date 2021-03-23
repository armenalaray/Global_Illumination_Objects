/* date = March 20th 2021 2:04 pm */

#ifndef MATRIX_H
#define MATRIX_H

#include <vector>
#include <array>
#include <initializer_list>
#include <string>
#include <iostream>
#include <fstream>
#include <ios>
#include <cassert>


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
    
    const size_t operator()(const size_t row, const size_t col)const{return strides[0]*row+col;}
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
    
    const size_t operator()(const size_t n)const{return n;}
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
    
    Matrix():
    desc{},
    elem{}
    {}
    
    Matrix(const size_t i, const size_t j):
    desc{i,j},
    elem(desc.size())
    {}
    
    T& operator()(const size_t row_i, const size_t col_i);
    const T& operator()(const size_t row_i, const size_t col_i)const;
    size_t get_extent(size_t i)const{return desc.extent(i);}
    
    Matrix& operator-=(const Matrix&);
    
    void debug_print(std::string)const;
    Matrix<T,2>& make_diagonal(const Matrix<T,1> & o);
    Matrix<T,2>& make_identity();
    
    private:
    Matrix_desc<2> desc;
    std::vector<T> elem;
};

template<typename T>
std::ostream& operator<<(std::ostream& o, const Matrix<T,2>& m){
    for(size_t i = 0; i < m.get_extent(0); ++i){
        for(size_t j = 0; j < m.get_extent(1); ++j){
            o<<m(i,j)<<" ";
        }
        o<<std::endl;
    }
    return o;
}

template<typename T>
Matrix<T,2>& Matrix<T,2>::operator-=(const Matrix<T,2>& o){
    assert(o.get_extent(0) == get_extent(0) &&
           o.get_extent(1) == get_extent(1));
    auto j=o.elem.begin();
    for(auto i=elem.begin();i!=elem.end();++i,++j)*i-=*j;
    return *this;
}

template<typename T>
Matrix<T,2>& Matrix<T,2>::make_diagonal(const Matrix<T,1>& o){
    assert(o.get_extent() == get_extent(0) &&
           o.get_extent() == get_extent(1));
    for (size_t i = 0; i < get_extent(0); ++i)(*this)(i,i) = o(i);
    return *this;
}

template<typename T>
Matrix<T,2>& Matrix<T,2>::make_identity(){
    assert(get_extent(0) == get_extent(1));
    for (size_t i = 0; i < get_extent(0); ++i)(*this)(i,i) = T{1};
    return *this;
}

template<typename T>
T& Matrix<T,2>::operator()(const size_t row_i, const size_t col_i){
    return elem[desc(row_i,col_i)];
}

template<typename T>
const T& Matrix<T,2>::operator()(const size_t row_i, const size_t col_i)const{
    return elem[desc(row_i,col_i)];
}

template<typename T>
Matrix<T,2> mult_m(const Matrix<T,2>& m1,const Matrix<T,2>& m2){
    assert(m1.get_extent(1)==m2.get_extent(0));
    Matrix<T,2> res(m1.get_extent(0), m2.get_extent(1));
    for(auto i=0;i<m1.get_extent(0);++i){
        for(auto k=0;k<m2.get_extent(1);++k){
            for(auto j=0;j<m2.get_extent(0);++j){
                res(i,k) += m1(i,j)*m2(j,k);
            }
        }
    }
    return res;
}

template<typename T>
Matrix<T,2> sub_m(const Matrix<T,2>& m1,const Matrix<T,2>& m2){
    assert(m1.get_extent(0)==m2.get_extent(0) &&  m1.get_extent(1)==m2.get_extent(1));
    Matrix<T,2> res{m1};
    res-=m2;
    return res;
}

template<typename T>
void num_solver_gs(const Matrix<T,2>& a, Matrix<T,1>& x,const Matrix<T,1>& b){
    for(size_t i=0;i<a.get_extent(0);++i){
        T s{};
        T ic{1.0f/a(i,i)};
        for (size_t j=0;j<a.get_extent(1);++j){
            if(j!=i)
                s+=a(i,j)*x(j);
        }
        s=-s;
        x(i)=(b(i)+s)*ic;
    }
}


template<typename T>
void Matrix<T,2>::debug_print(std::string fn)const{
    std::ofstream ofs;
    ofs.open(fn, std::ios::trunc | std::ios::out);
    if(ofs.is_open())
    {
        /* 
        First goes width then height
         */
        ofs << "P3\n" << desc.extent(1) << " " << desc.extent(0) << "\n255\n";
        for(size_t i = 0; i < desc.extent(0); ++i){
            for(size_t j = 0; j < desc.extent(1); ++j){
                T Value = (*this)(i,j);
                //FFValue /= MaxPixVal;
                int ir = 0;
                int ig = 0;
                int ib = 0;
                
                if(Value < 0)
                {
                    ir = 255;
                    ig = 0;
                    ib = 0;
                }
                else if(Value > 1.0f)
                {
                    ir = 0;
                    ig = 0;
                    ib = 255;
                }
                /*
                else if(IsInSamePlane(i,j))
                {
                    ir = 0;
                    ig = 255;
                    ib = 0;
                }
                */
                else
                {
                    ir = int(255.99 * Value);
                    ig = int(255.99 * Value);
                    ib = int(255.99 * Value);
                }
                ofs << std::to_string(ir) << " " << std::to_string(ig) << " " << std::to_string(ib) << std::endl;
            }
        }
        ofs.close();
    }
    else std::cout << "Unable to open file:" << fn << std::endl;
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
    
    Matrix():
    desc{},
    elem{}
    {}
    
    Matrix(const size_t i):
    desc{i},
    elem(desc.size())
    {}
    
    T& operator()(const size_t n);
    const T& operator()(const size_t n)const;
    size_t get_extent()const{return desc.extent();}
    
    Matrix& operator-=(const Matrix&);
    
    void debug_print(std::string)const;
    T squared_norm();
    private:
    Matrix_desc<1> desc;
    std::vector<T> elem;
};


template<typename T>
Matrix<T,1>& Matrix<T,1>::operator-=(const Matrix<T,1>& o){
    assert(o.get_extent() == get_extent());
    auto j=o.elem.begin();
    for(auto i=elem.begin();i!=elem.end();++i,++j)*i-=*j;
    return *this;
}


template<typename T>
T Matrix<T,1>::squared_norm(){
    T res{};
    for(auto i=elem.begin();i!=elem.end();++i)res+=*i**i;
    return res;
}

template<typename T>
T& Matrix<T,1>::operator()(const size_t n){
    return elem[desc(n)];
}

template<typename T>
const T& Matrix<T,1>::operator()(const size_t n)const{
    return elem[desc(n)];
}

template<typename T>
Matrix<T,1> mult_m(const Matrix<T,2>& m1,const Matrix<T,1>& m2){
    assert(m1.get_extent(1)==m2.get_extent());
    Matrix<T,1> res(m1.get_extent(0));
    for(auto i=0;i<m1.get_extent(0);++i){
        for(auto j=0;j<m2.get_extent();++j){
            res(i) += m1(i,j)*m2(j);
        }
    }
    return res;
}

template<typename T>
Matrix<T,1> sub_m(const Matrix<T,1>& m1,const Matrix<T,1>& m2){
    assert(m1.get_extent()==m2.get_extent());
    Matrix<T,1> res{m1};
    res-=m2;
    return res;
}

template<typename T>
void Matrix<T,1>::debug_print(std::string fn)const{
    std::ofstream ofs;
    ofs.open(fn, std::ios::trunc | std::ios::out);
    if(ofs.is_open())
    {
        /* 
        First goes width then height
         */
        ofs << "P3\n" << std::to_string(1) << " " << desc.extent() << "\n255\n";
        for(size_t i = 0; i < desc.extent(); ++i){
            T Value = (*this)(i);
            //FFValue /= MaxPixVal;
            int ir = 0;
            int ig = 0;
            int ib = 0;
            
            if(Value < 0)
            {
                ir = 255;
                ig = 0;
                ib = 0;
            }
            else if(Value > 1.0f)
            {
                ir = 0;
                ig = 0;
                ib = 255;
            }
            /*
            else if(IsInSamePlane(i,j))
            {
                ir = 0;
                ig = 255;
                ib = 0;
            }
            */
            else
            {
                ir = int(255.99 * Value);
                ig = int(255.99 * Value);
                ib = int(255.99 * Value);
            }
            ofs << std::to_string(ir) << " " << std::to_string(ig) << " " << std::to_string(ib) << std::endl;
        }
        ofs.close();
    }
    else std::cout << "Unable to open file:" << fn << std::endl;
}


#endif //MATRIX_H
