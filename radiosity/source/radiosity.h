/* date = March 20th 2021 4:49 pm */

#ifndef RADIOSITY_H
#define RADIOSITY_H

#include <string>

#include "matrix.h"
#include "hemi_cube.h"
#include "element.h"
#include "space.h"

extern Space space;

class Radiosity{
    public:
    Radiosity();
    private:
    Matrix<float,2> f;
};

#endif //RADIOSITY_H
