/*
// NOTE(Alex): Light transport algorithm 
subsurface scattering.

T = e^(-(sigma_a + sigma_s)x);

p(x) = -sigma_t * e^(-sigma_t * x), where 0 < x < inf.

////////////////////////////////////////////////////////
Pseudocode:

Initial conditions x(position), v(direction)

:NEWPOSITION

Compute new position x+= v*free path length

Did the photon exit?

YES - Stop Simulation 

NO - Has the photon been absorbed?

YES - Stop Simulation
 
NO - The photon is scattered, compute new direction (i.e. update v)

GOTO NEWPOSITION

////////////////////////////////////////////////////////


so first i only gonna get the ratios of absorbed, transmitted and reflected.
for different sigma_a, sigma_s, and slab width, 

// NOTE(Alex): we will use the slab method.
// NOTE(Alex): We will define the bottom as v.z

///////////////////////////////////////////////////////////////

MC RESULTS
MC Trasmitted   0.810320
MC Reflected    0.116210
MC Absorbed     0.073470
Total           1.000000

static float d = 0.2f;
static float SigmaA = 0.1f;
static float SigmaS = 2.0f;

///////////////////////////////////////////////////////////////
MC RESULTS
MC Trasmitted   0.585530
MC Reflected    0.231650
MC Absorbed     0.182820
Total           1.000000

static float d = 1.0f;
static float SigmaA = 0.1f;
static float SigmaS = 2.0f;

///////////////////////////////////////////////////////////////

MC RESULTS
MC Trasmitted   0.387890
MC Reflected    0.300690
MC Absorbed     0.311420
Total           1.000000

static float d = 2.0f;
static float SigmaA = 0.1f;
static float SigmaS = 2.0f;

///////////////////////////////////////////////////////////////

MC RESULTS
MC Trasmitted   0.227570
MC Reflected    0.046590
MC Absorbed     0.725840
Total           1.000000

static float d = 1.0f;
static float SigmaA = 0.5f;
static float SigmaS = 0.5f;


*/


#include "pathtracer.h"

// NOTE(Alex): This means that in 1 meter there will be e^0.1 abosrption of radiance 
//static float SigmaA = 0.1f;
//static float SigmaS = 1.0f;

static float g = 0.75f;
// NOTE(Alex): 1 meter

static float d_ = 0.1f;
static float z0 = 0.0f;
static float z1 = d_;

static float SigmaA = 1.0f;
static float SigmaS = 2.0f;

static float SigmaT = SigmaA + SigmaS;
static float InverseSigmaT = 1 / SigmaT;

void HGICDF(float & CT)
{
    if(g == 0)
    {
        CT = 2.0f * RandNum0ToLessThan1() - 1.0f;
    }
    else
    {
        float mu = (1.0f - g*g) / (2.0f * g * RandNum0ToLessThan1() + 1.0f - g); 
        CT = (1.0f + g*g - mu * mu) / (2.0f * g); 
    }
}

float HGPDF(float CT)
{
    if(g == 0)
        return 1.0f;
    else
        return (1 - (g * g)) / (4.0f * (float)M_PI * pow(1 + g*g - 2*g*CT, 3/2));
}

void BeerLambertICDF(float & x)
{
    // NOTE(Alex): Calculate step size
    x = -logf(1.0f - RandNum0ToLessThan1()) * InverseSigmaT;
}

float BeerLambertPDF(float x)
{
    return SigmaT * exp(-SigmaT*x);
}

void WriteToFile(char * FileName, vec3 * Map, int Width, int Height)
{
    std::ofstream ofs; 
    ofs.open(FileName, std::ios::out | std::ios::binary); 
    ofs << "P3\n" << Width << " " << Height << "\n255\n";
    for(int j = Height - 1; j >= 0; j--)
    {
        for(int i = 0; i < Width; i++)
        {
            vec3 & Col = Map[j * Width + i];
            int ir = int(255.99*Col.r);
            int ig = int(255.99*Col.g);
            int ib = int(255.99*Col.b);
            ofs << ir << " " << ig << " " << ib << endl;
        }
    }
    ofs.close();
}


int main()
{
    const uint32_t SimCount = 1;
    uint32_t PhotonCount = 10000; //1 Milion photons
    
#define DIM10  1
#ifdef DIM1
    float MinX = -0.5f;
    float MaxX = 0.5f;
    float MinY = -0.5f;
    float MaxY = 0.5f;
#elif DIM10
    float MinX = -5.f;
    float MaxX = 5.f;
    float MinY = -5.f;
    float MaxY = 5.f;
#elif DIM5
    float MinX = -2.5f;
    float MaxX = 2.5f;
    float MinY = -2.5f;
    float MaxY = 2.5f;
#endif
    
    float m = 5.0f; // 5 chambers in pistol.
    
    const uint32_t Width = 512, Height = 512; 
    vec3 * TMap = new vec3[Width * Height]; 
    vec3 * RMap = new vec3[Width * Height]; 
    
    vec3 * MeansT = new vec3[Width * Height];
    vec3 * MeansR = new vec3[Width * Height];
    memset(TMap, 0x0, sizeof(vec3) * Width * Height); 
    memset(RMap, 0x0, sizeof(vec3) * Width * Height); 
    
    float EmissorRadius = 1.0f;
    
    for(uint32_t SimIndex = 1; SimIndex <= SimCount; SimIndex++)
    {
        memset(MeansT, 0x0, sizeof(vec3) * Width * Height); 
        memset(MeansR, 0x0, sizeof(vec3) * Width * Height); 
        float RC = 0;
        float TC = 0;
        float AC = 0;
        
        float RAbsorbed = SigmaA / SigmaT;
        
        float MaxDensityT = 0.0f;
        float MaxDensityR = 0.0f;
        
        //float d = ((float)SimIndex / (float)SimCount) * d_;
        for(uint32_t PhotonIndex = 0; PhotonIndex < PhotonCount; PhotonIndex++)
        {
            float Weight = 1.0f;
            
            //vec3 InitPos = EmissorRadius * RandomInUnitDisc();
            //vec3 P = InitPos;
            vec3 P(0,0,0);
            vec3 V(0,0,1);
            vec3 Temp = P;
            uint32_t Iter = 0;
            while(Weight != 0.0f)
            {
                // NOTE(Alex): Absorbed
                //if(RandNum0ToLessThan1() < RAbsorbed)
                //{
                //AC += Weight;
                //break;
                //}
                
                AC += RAbsorbed;
                Weight -= RAbsorbed;
                Weight = std::max<float>(0,Weight);
                
                if(Weight < 0.001)
                {
                    if(RandNum0ToLessThan1() > (1.0f / m))
                    {
                        break;
                    }
                    else
                    {
                        Weight *= m;
                    }
                }
                
                // NOTE(Alex): Scattering
                // NOTE(Alex): CT is cos(theta)
                float CT = 0.0f;
                HGICDF(CT);
                //CT = CT / HGPDF(CT);
                float ST = sqrtf(1.0f - CT*CT);
                
                vec3 K = V;
                vec3 AP = MakeUnitVector(Cross(vec3(0,1,0), K));
                vec3 A = MakeUnitVector(CT * K  + ST * AP); 
                
                // NOTE(Alex): Spin V
                float PHI = 2.0f * (float)M_PI * RandNum0ToLessThan1();
                float CPHI = cos(PHI);
                float SPHI = sin(PHI);
                V = A * CPHI + (1.0f - CPHI)*Dot(A,K)*K + SPHI*Cross(K,A); 
                V.MakeUnitVector();
                
                
                // NOTE(Alex): Update position
                float x = 0.0f;
                BeerLambertICDF(x);
                //P += V * (x / BeerLambertPDF(x));
                
                Temp += V * x;
                
                // NOTE(Alex): Check if outside the slab
                if(Temp.z > d_)
                {
                    float t1 = (z1 - P.z) / V.z;
                    vec3 P1 = P + V*t1;
                    
                    //Assert(P1.x >= -5.0f && P1.x <= 5.0f);
                    //Assert(P1.y >= -5.0f && P1.y <= 5.0f);
                    if(P1.x >= MinX && P1.x <= MaxX && P1.y >= MinY && P1.y <= MaxY)
                    {
                        float Nx = (-MinX + P1.x) / (MaxX - MinX);
                        float Ny = (-MinY + P1.y) / (MaxY - MinY);
                        
                        int Sx = (int)(Nx * Width);
                        int Sy = (int)(Ny * Height);
                        
                        vec3 & t = TMap[Width * Sy + Sx]; 
                        t += vec3(Weight, Weight, Weight); 
                        if(t.x > MaxDensityT) MaxDensityT = t.x;
                    }
                    
                    TC += Weight;
                    break;
                }
                else if(Temp.z < 0)
                {
                    float t0 = (z0 - P.z) / V.z;
                    vec3 P0 = P + V*t0;
                    
                    //Assert(P0.x >= -5.0f && P0.x <= 5.0f);
                    //Assert(P0.y >= -5.0f && P0.y <= 5.0f);
                    if(P0.x >= MinX && P0.x <= MaxX && P0.y >= MinY && P0.y <= MaxY)
                    {
                        float Nx = (-MinX + P0.x) / (MaxX - MinX);
                        float Ny = (-MinY + P0.y) / (MaxY - MinY);
                        
                        int Sx = (int)(Nx * Width);
                        int Sy = (int)(Ny * Height);
                        
                        vec3 & t = RMap[Width * Sy + Sx]; 
                        t += vec3(Weight, Weight, Weight); 
                        if(t.x > MaxDensityR) MaxDensityR = t.x;
                    }
                    
                    RC += Weight;
                    break;
                }
                
                P = Temp;
                ++Iter;
            }
        }
        
        float MCRT = (float)TC / (float)PhotonCount;
        float MCRR = (float)RC / (float)PhotonCount;
        float MCRA = (float)AC / (float)PhotonCount;
        printf("------------------------------------------------------\n");
        printf("PARAMETERS\n");
        printf("Slab width    %10f\n", d_);
        printf("g             %10f\n", g);
        printf("SigmaS        %10f\n", SigmaS);
        printf("SigmaA        %10f\n", SigmaA);
        printf("------------------------------------------------------\n");
        printf("MC RESULTS\n");
        printf("Theorical Trasmitted %10f\n", BeerLambertPDF(d_));
        printf("MC Trasmitted %10f\n", MCRT);
        printf("MC Reflected  %10f\n", MCRR);
        printf("MC Absorbed   %10f\n", MCRA);
        printf("Total         %10f\n", MCRT + MCRR + MCRA);
        printf("Max DensityT  %10f\n", MaxDensityT);
        printf("Max DensityR  %10f\n", MaxDensityR);
        
        // NOTE(Alex): Expected value of X
        //for(uint32_t i = 0; i < Width * Height; i++)
        //{
        //TMap[i] /= MaxDensityT;
        //RMap[i] /= MaxDensityR;
        //TMap[i] /= (float)PhotonCount;
        //RMap[i] /= (float)PhotonCount;
        //MeansT[i] += TMap[i];
        //MeansR[i] += RMap[i];
        //}
        
        
        for(uint32_t i = 0; i < Width * Height; i++)
        {
            MeansT[i] = TMap[i] / (float)SimIndex;
            MeansR[i] = RMap[i] / (float)SimIndex;
            MeansT[i] = vec3(std::min(1.0f, MeansT[i].x),std::min(1.0f, MeansT[i].y),std::min(1.0f, MeansT[i].z));
            MeansR[i] = vec3(std::min(1.0f, MeansR[i].x),std::min(1.0f, MeansR[i].y),std::min(1.0f, MeansR[i].z));
        }
        
#if 1        
        char SSSTFileName[200] = {};
        snprintf(SSSTFileName, ArrayCount(SSSTFileName), "SSS_Pass_%i.ppm", SimIndex);
        
        char SSSRFileName[200] = {};
        snprintf(SSSRFileName, ArrayCount(SSSRFileName), "SSS_Pass_%i.ppm", SimIndex);
#else
        char SSSTFileName[200] = {};
        snprintf(SSSTFileName, ArrayCount(SSSTFileName), "SSS_T_d__%f.ppm", d_);
        
        char SSSRFileName[200] = {};
        snprintf(SSSRFileName, ArrayCount(SSSRFileName), "SSS_R_d_%f.ppm", d_);
#endif
        
        WriteToFile(SSSTFileName, MeansT, Width, Height);
        WriteToFile(SSSRFileName, MeansR, Width, Height);
    }
    
    delete[] TMap; 
    delete[] RMap;
    
    delete[] MeansT;
    delete[] MeansR;
}



