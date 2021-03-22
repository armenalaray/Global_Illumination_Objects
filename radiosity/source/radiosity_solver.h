/* date = March 21st 2021 10:02 am */

#ifndef RADIOSITY_SOLVER_H
#define RADIOSITY_SOLVER_H

#include "matrix.h"

class Radiosity_solver{
    public:
    virtual Matrix_2d<float> request_ff_matrix() = 0;
};

#endif //RADIOSITY_SOLVER_H
