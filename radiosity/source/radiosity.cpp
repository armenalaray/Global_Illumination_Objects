#include "radiosity.h"

/* 
Radiosity Constructor
Description:
The radiosity solver contains three stimuli objects, 
each of them solves a system o linear equations with Form Factor 
previously calculated by Element Objects.

Parameters: 
float fw: Face Size Width.
 int hps: Hitables Per Face Side.

Output: -
 */
Radiosity::Radiosity(float fw, int hps):
qm{fw,hps},
f(qm.calc_ff()),
r_s{5, fw, hps, f, 15.0f, 0.73f, 0.12f, 0.73f, 0.65f, 0.73f},
g_s{5, fw, hps, f, 15.0f, 0.73f, 0.45f, 0.73f, 0.05f, 0.73f},
b_s{5, fw, hps, f, 15.0f, 0.73f, 0.15f, 0.73f, 0.05f, 0.73f}
{
    std::string FString = "F" + std::to_string(0) + "_matrix.ppm";
    f.debug_print(FString);
    
    {
        /* 
         Red stimuli
         */
        std::string PString = "P_r_s_matrix.ppm";
        r_s.p.debug_print(PString);
        
        std::string KString = "K_r_s_matrix.ppm";
        r_s.k.debug_print(KString);
        
        std::string ResidualString = "Residual_r_s_matrix.ppm";
        r_s.residual.debug_print(ResidualString);
        
        std::string BString = "B_r_s_matrix.ppm";
        r_s.b.debug_print(BString);
    }
    {
        /* 
        Green stimuli
         */
        std::string PString = "P_g_s_matrix.ppm";
        g_s.p.debug_print(PString);
        
        std::string KString = "K_g_s_matrix.ppm";
        g_s.k.debug_print(KString);
        
        std::string ResidualString = "Residual_g_s_matrix.ppm";
        g_s.residual.debug_print(ResidualString);
        
        std::string BString = "B_g_s_matrix.ppm";
        g_s.b.debug_print(BString);
    }
    {
        /* 
         Blue stimuli
         */
        std::string PString = "P_b_s_matrix.ppm";
        b_s.p.debug_print(PString);
        
        std::string KString = "K_b_s_matrix.ppm";
        b_s.k.debug_print(KString);
        
        std::string ResidualString = "Residual_b_s_matrix.ppm";
        b_s.residual.debug_print(ResidualString);
        
        std::string BString = "B_b_s_matrix.ppm";
        b_s.b.debug_print(BString);
    }
    
    qm.move_radiosities(r_s.b,g_s.b,b_s.b);
}


