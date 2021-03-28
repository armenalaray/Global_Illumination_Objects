/* date = March 23rd 2021 5:40 pm */

#ifndef STIMULI_H
#define STIMULI_H

#include <string>
#include "matrix.h"


/* 
class Stimuli
referenced by: class Radiosity
Solver for K B = E 
 */

class Stimuli{
    public:
    Stimuli(int fc, float fw, int hps, const Matrix<float,2>& f, float e_s, float f0_s, float f1_s, float f2_s, float f3_s, float f4_s);
    size_t n;
    Matrix<float,1> b;
    Matrix<float,1> residual;
    Matrix<float,2> p;
    Matrix<float,2> k;
};

#endif //STIMULI_H
