#include "radiosity.h"

Radiosity::Radiosity():
f{}
{
    f=space.request_ff_matrix();
    
    std::string FString = "F" + std::to_string(0) + "_matrix.ppm";
    f.debug_print(FString);
    
    auto n=f.get_extent(0);
    Matrix<float,1> vp(n);
    Matrix<float,1> e(n);
    Matrix<float,1> b(n);
    Matrix<float,1> residual(n);
    Matrix<float,2> p(n,n);
    Matrix<float,2> k(n,n);
    Matrix<float,2> m(n,n);
    
    m.make_identity();
    p.make_diagonal(vp);
    
    std::string PString = "P" + std::to_string(0) + "_matrix.ppm";
    p.debug_print(PString);
    k= sub_m(m,mult_m(p, f));
    
    std::string KString = "K" + std::to_string(0) + "_matrix.ppm";
    k.debug_print(KString);
    
    for(;;)
    {
        residual=sub_m(e,mult_m(k,b));
        std::string ResidualString = "Residual" + std::to_string(0) + "_matrix.ppm";
        residual.debug_print(ResidualString);
        
        float norm = residual.squared_norm();
        if(norm < 0.1f)
        {
            break;
        }
        
        num_solver_gs(k, b, e);
        
        std::string BString = "B" + std::to_string(0) + "_matrix.ppm";
        b.debug_print(BString);
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
