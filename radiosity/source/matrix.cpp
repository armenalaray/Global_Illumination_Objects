#include "matrix.h"

Matrix_desc_2d::Matrix_desc_2d(const size_t rc, const size_t cc):
extents{rc,cc},
strides{rc,0}
{}

size_t Matrix_desc_2d::operator()(const size_t row, const size_t col){
    return strides[0]*row+strides[1]*col;
}