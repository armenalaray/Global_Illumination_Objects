/*
// NOTE(Alex): we are going to make radiosity from scratch.
Cornell box 0, 5 all sides 5 x 5 x 5
light 2,3 ( es uno de los elementos ) 

Neceisto mapear los elementos a los vertices algo como

vertexIndices, aha!!




*/

#include "pathtracer.h"

int main()
{
    // TODO(Alex): Haz un mesheo mas pequeñito y checa si la suma de mis factores forma es igual a uno!!
    // Checa si el error va a bajando conforme el mesheo se hace mas pequeñito. Si as asi el form factor es correcto!!!
    
#if 0  
    
    matrix<radiosity_float> VP(3,1);
    for(int i = 0; i < 3; i += 1)
    {
        VP[i][0] = 0.5f;
    }
    
    matrix<radiosity_float> I(3,3);
    I.MakeDiagonal(VP);
    
    matrix<radiosity_float> A(3,3);
    A[0][0] = 2.0f;
    A[0][1] = 1.0f;
    A[0][2] = 1.0f;
    A[1][0] = 1.0f;
    A[1][1] = 2.0f;
    A[1][2] = 1.0f;
    A[2][0] = 1.0f;
    A[2][1] = 1.0f;
    A[2][2] = 2.0f;
    
    matrix<radiosity_float> x(3,3);
    x[0][0] = 1.0f;
    x[1][0] = 1.0f;
    x[2][0] = 1.0f;
    
    x[0][1] = 1.0f;
    x[1][1] = 0.0f;
    x[2][1] = 0.0f;
    
    x[0][2] = 0.0f;
    x[1][2] = 1.0f;
    x[2][2] = 0.0f;
    
    cout << I << endl;
    cout << A << endl;
    
    matrix<radiosity_float> b(3,1);
    b = I * A;
    cout << b << endl;
    
    //b = A * x ;
    //b = I - b;
    //cout << b << endl;
#else
    size_t ElementsPerSide = 10;
    float FaceWidth = 20.0f;
    radiosity_mesh Mesh(ElementsPerSide,FaceWidth);
    Mesh.CalculateRadiosities();
#endif
    
    
}


