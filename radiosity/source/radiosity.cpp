#include "radiosity.h"

void Radiosity::CalculateFFs(){
    HemiCube hm{100,100};
    Element elem{};
    while(hm.get_next_element(elem))
    {
        /* 
        Cuando sacas un rayo, ese rayo te dice el indice, entonces, esa informacion ya sea que
        la regreso por medio de mi interface, pero ff matrix, tiene que ser llenada 
         */
        
        Matrix<float,1> row = elem.calc_ff(hm);
        ffs.row(elem.i) = row; 
    }
}
