/* date = March 20th 2021 4:49 pm */

#ifndef RADIOSITY_H
#define RADIOSITY_H

#include "matrix.h"
#include "hemi_cube.h"
#include "element.h"
#include "space.h"

extern Space space;

class Radiosity{
    public:
    Radiosity();
    private:
    Matrix_2d<float> ffs;
};

#endif //RADIOSITY_H
