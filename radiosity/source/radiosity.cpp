#include "radiosity.h"

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
}



#if 0
void CalculateRadiosities()
{
    for (size_t FaceIndex = 0; FaceIndex < FaceCount; FaceIndex += 1){
        for(size_t i = 0; i < HitablePerSide; i += 1){
            for(size_t j = 0; j < HitablePerSide; j += 1){
                size_t Index = FaceIndex * ElementsPerFace + i * ElementsPerSide + j;
                
                if(FaceIndex == 0)
                {
                    ChannelData[0].VP[Index][0] = 0.73f;
                    ChannelData[1].VP[Index][0] = 0.73f;
                    ChannelData[2].VP[Index][0] = 0.73f;
                }
                else if(FaceIndex == 1)
                {
                    ChannelData[0].VP[Index][0] = 0.12f;
                    ChannelData[1].VP[Index][0] = 0.45f;
                    ChannelData[2].VP[Index][0] = 0.15f;
                }
                else if(FaceIndex == 2)
                {
                    
                    ChannelData[0].VP[Index][0] = 0.73f;
                    ChannelData[1].VP[Index][0] = 0.73f;
                    ChannelData[2].VP[Index][0] = 0.73f;
                }
                else if(FaceIndex == 3)
                {
                    ChannelData[0].VP[Index][0] = 0.65f;
                    ChannelData[1].VP[Index][0] = 0.05f;
                    ChannelData[2].VP[Index][0] = 0.05f;
                }
                else if(FaceIndex == 4)
                {
                    ChannelData[0].VP[Index][0] = 0.73f;
                    ChannelData[1].VP[Index][0] = 0.73f;
                    ChannelData[2].VP[Index][0] = 0.73f;
                }
            }
        }
    }
    
    ChannelData[0].E[n-1][0] = 15.0f;
    ChannelData[1].E[n-1][0] = 15.0f;
    ChannelData[2].E[n-1][0] = 15.0f;
    
    // NOTE(Alex): asi no funciona, como creas la data aqui!!! osea!!!
    CreateHitables();
    CreateOneElementPerHitable();
    CalculateFormFactors<radiosity_float>();
    
    for(int ChannelIndex = 0; ChannelIndex < 3; ChannelIndex += 1)
    {
        matrix<radiosity_float> & VP = ChannelData[ChannelIndex].VP;
        matrix<radiosity_float> & E = ChannelData[ChannelIndex].E;
        matrix<radiosity_float> & B = ChannelData[ChannelIndex].B;
        
        matrix<radiosity_float> Residual(n,1);
        
        matrix<radiosity_float> P(n,n);
        P.MakeDiagonal(VP);
        
        string PString = "P" + to_string(ChannelIndex) + "_matrix.ppm";
        P.DebugPrint(PString);
        
        K = M - P * F;
        
        string KString = "K" + to_string(ChannelIndex) + "_matrix.ppm";
        K.DebugPrint(KString);
        
        for(;;)
        {
            Residual = E - K * B;
            string ResidualString = "Residual" + to_string(ChannelIndex) + "_matrix.ppm";
            Residual.DebugPrint(ResidualString);
            
            radiosity_float Norm = Residual.SquaredNorm();
            if(Norm.Value < 0.1f)
            {
                break;
            }
            NumSolverGS(K, B, E);
            
            string BString = "B" + to_string(ChannelIndex) + "_matrix.ppm";
            ChannelData[ChannelIndex].B.DebugPrint(BString);
        }
    }
    
    RenderLightMaps();
}
#endif
