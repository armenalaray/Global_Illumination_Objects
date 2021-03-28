#include "stimuli.h"

/* 
 Stimuli Constructor
Description:
Stimuli object solves K B = E using pre-calculated Form-Factor data as input.
The solver uses Gauss-Seidel. NOTE: Look at matrix.h for Gauss-Seidel Solver.
Parameters: 
int fc: FaceCount - 5 for Cornell Box scene. 
float fw: FaceWidth
int hps: Hitables Per Face Side.
const Matrix<float:2>& f: Form-Factor matrix previously pre-calculated.
float e_s: Average Emissivity value for Element N-1. This is the area light in Cornell-Box.
float f0_s: Reflectivity value for Face XY_Z0
float f1_s: Reflectivity value for Face YZ_X0
float f2_s: Reflectivity value for Face XZ_Y0
float f3_s: Reflectivity value for Face YZ_X5
float f4_s: Reflectivity value for Face XZ_Y5

Output: -
 */
Stimuli::Stimuli(int fc, float fw, int hps, const Matrix<float,2>& f, float e_s, float f0_s, float f1_s, float f2_s, float f3_s, float f4_s):
n{f.get_extent(0)},
b(n),
residual(n),
p(n,n),
k(n,n)
{
    int hpf=hps*hps;
    Matrix<float,1> vp(n);
    Matrix<float,1> e(n);
    Matrix<float,2> m(n,n);
    for (size_t fi = 0; fi < fc; fi += 1){
        for(size_t i = 0; i < hps; i += 1){
            for(size_t j = 0; j < hps; j += 1){
                size_t Index = fi * hpf + i * hps + j;
                switch(fi){
                    case 0:
                    {
                        vp(Index) = f0_s;
                    }break;
                    case 1:
                    {
                        vp(Index) = f1_s;
                    }break;
                    case 2:
                    {
                        vp(Index) = f2_s;
                    }break;
                    case 3:
                    {
                        vp(Index) = f3_s;
                    }break;
                    case 4:
                    {
                        vp(Index) = f4_s;
                    }break;
                }
            }
        }
    }
    e(n-1)=e_s;
    m.make_identity();
    p.make_diagonal(vp);
    k= sub_m(m,mult_m(p, f));
    for(;;){
        residual=sub_m(e,mult_m(k,b));
        float norm = residual.squared_norm();
        if(norm < 0.1f)
            break;
        num_solver_gs(k, b, e);
    }
}