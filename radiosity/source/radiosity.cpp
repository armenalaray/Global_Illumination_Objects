#include "radiosity.h"

Radiosity::Radiosity():
ffs{}
{
    ffs = space.request_ff_matrix();
}
