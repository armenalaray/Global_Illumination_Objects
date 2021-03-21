/* date = March 20th 2021 4:49 pm */

#ifndef RADIOSITY_H
#define RADIOSITY_H

#include "matrix.h"
#include "hemi_cube.h"
#include "element.h"

class Radiosity{
    public:
    Radiosity()=default;
    void CalculateFFs();
    private:
    Matrix<float,2> ffs;
};

#endif //RADIOSITY_H
