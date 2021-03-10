#include <iostream>

#include "radiosity_matrix.h"

int main()
{
    
#if 0    
    radiosity_matrix<double, 0> m0{1}; //zero dimensions: a scalar
    radiosity_matrix<double, 1> m1{1,2,3,4}; //one dimension: a vector(4,elements)
    radiosity_matrix<double, 2> m2{
        {00,01,02,03}, //row0 
        {10,11,12,13}, //row1
        {20,21,22,23}, //row2
    };
    radiosity_matrix<double,3> m3(4,7,9); //three dimenisons(4*7*9), all 0-initialized
    radiosity_matrix<complex<double>,17> m17; //17 dimendions (no elements so far)
    
    radiosity_matrix<double,2> md;
    radiosity_matrix<string,2> ms; //OK: just don´t try arithmetic operations
    
    radiosity_matrix<radiosity_matrix<int,2>,3> mm
    {
        {//row 0
            {{1,2},{3,4}}, //col 0
            {{4,5},{6,7}}, //col 1
        },
        {//row 1
            {{1,2},{3,4}}, //col 0
            {{4,5},{6,7}}, //col 1
        },
        {//row 2
            {{1,2},{3,4}}, //col 0
            {{4,5},{6,7}}, //col 1
        }
    };
    
    radiosity_matrix<char,2> mc1(2,3,4); //error: too many dimension sizes
    radiosity_matrix<char,2> mc2 //error: initializer missing for second dimension
    {
        {'1','2','3'}
    };
    
    radiosity_matrix<char,2> mc3 //error: element missing for third column
    {
        {'1','2','3'}
        {'1','2'}
    };
#endif
    
    
    radiosity_matrix<double, 1> m1(100); //one dimension: a vector(100 elements)
    radiosity_matrix<double, 2> m1(50,6000); //two dimensions: 50*6000 elements
    
    auto d1 = m1.order; //1 Orders are dimensions
    auto d2 = m2.order; //2
    
    auto e1 = m1.extent(0); // 100
    auto e2 = m1.extent(1); // error: m1 is one-dimensional
    
    auto e3 = m2.extent(0); // 50
    auto e4 = m2.extent(1); // 6000
    
    auto s1 = m1.size(); //100
    auto s2 = m2.size(); //50*6000
    
    
}