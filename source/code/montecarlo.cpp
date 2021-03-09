/*
Alejandro Armenta Ayala
This is a montecarlo program that stimates pi
Its gonna approach to the right value!
*/

#include <limits>
#include "pathtracer.h"

typedef std::numeric_limits<double> dbl;
typedef std::numeric_limits<float> flt;


static const int BUCKETSIZE = 20;
static const int MAX_FREQ = 50;

double pdf_sphere(vec3 v)
{
    return 1.0 / 4.0 * M_PI;
}

double pdf_nonuniform(double x)
{
    if(x > 0)
        return 0.5 * x;
    else
        return 0.00000001;
}

double pdf_uniform(double x)
{
    return 0.5;
}

double pdf_exact(double x)
{
    if(x > 0)
        return 3.0*x*x/8.0;
    else
        return 0.00000001;
}

float NormalDistributionPDF(float x)
{
    return 1.0f / sqrtf(2.0f * (float)M_PI) * exp(-x*x*0.5f);
}

inline uint64_t Fact(uint64_t x)
{
    return (x <= 1 ? 1 : x * Fact(x - 1));
}

int main(int argc, char ** argv)
{
    SeedInit();
    //#define MC 1
    //#define MCINT 1
    //#define MCINT_NONUNIFORM 1
    //#define MCINT_UNIFORM 1
    //#define MCINT_EXACT 1
    //#define MCINT_SPHERE 1
    
    //#define BERNOULLI_TRIAL 1
    //#define EXP_DICE 1
    
    //#define SAMPLING_DISTRIBUTION 1
    //#define PDF 1
    //#define TS_0 1
    //#define TS_1 1
    //#define CDF_DISCRETIZED 1
#ifdef MC    
    size_t Count = 1000000000;
    
    size_t InsideCircleCount = 0;
    cout.precision(dbl::max_digits10);
    //while()
    for(size_t i = 0; i < Count; i++)
    {
        float x = 2.0f * RandNum0ToLessThan1() - 1.0f;
        float y = 2.0f * RandNum0ToLessThan1() - 1.0f;
        // NOTE(Alex): You always are on the 1st quadrant, so you need to multiply by four the probability.
        if((x*x + y*y) < 1.0f)
            ++InsideCircleCount;
        if(i % 10000000 == 0)
            cout << "Pi estimate = " << 4.0 * static_cast<double>(InsideCircleCount) / static_cast<double>(Count) << std::endl;
    }
    
    //max_digits10 returns the number of decimal digits required to ensure that two distinct values of the type have distinct decimal representations!!!
    // Iterative Process Begin: 3.1464885351146488
    // Iterative Process End:   3.1415469707142281
    // PI:                      3.14159265358979323846264338327950288419
    // Stratified sampling:     3.1415911200000002
    // Best Estimate:           3.1415863831999999
    // Best Count:              10000000000
    /*

So it was reaching closer and closer pin pointing, and getting closer to the target!! Awesome!! 
we get quickly near pi, and then more slowly zero on it.
The law of Diminishing Returns
*/
#elif MCSTRAT
    /*
Stratified sampling!
*/
    
    size_t SamplesSide = 10000;
    size_t SamplesCount = SamplesSide * SamplesSide;
    size_t ICC = 0;
    size_t ICCStratified = 0;
    for(size_t j = 0; j < SamplesSide; j++)
    {
        for(size_t i = 0; i < SamplesSide; i++)
        {
            float x = 2.0f * RandNum0ToLessThan1() - 1.0f;
            float y = 2.0f * RandNum0ToLessThan1() - 1.0f;
            
            if((x*x + y*y) < 1.0f)
                ++ICC;
            
            x = 2.0f * ((i + RandNum0ToLessThan1()) / SamplesSide) - 1.0f;
            y = 2.0f * ((j + RandNum0ToLessThan1()) / SamplesSide) - 1.0f;
            
            if((x*x + y*y) < 1.0f)
                ++ICCStratified;
            
        }
    }
    
    cout.precision(dbl::max_digits10);
    cout << "Pi estimate = " << 4.0 * static_cast<double>(ICC) / static_cast<double>(SamplesCount) << std::endl;
    cout << "Pi estimate stratified = " << 4.0 * static_cast<double>(ICCStratified) / static_cast<double>(SamplesCount) << std::endl;
    
#elif MCINT
    /*
 Integral of x^2
 MCINT_EXACT =      2.6666666666666665 SampleCount  = 1
MCINT_UNIFORM =    2.6669427052875609  SampleCount = 1 000 000
MCINT_NONUNIFORM = 2.6669250232949175 SampleCount  =  700 000
*/
    
    /*Calculate the integral of x^2 from the range [0,2] numerically */
    
    int Count = 1000000;
    double Sum = 0.0f;
    for(int i = 0; i < Count; i++)
    {
        double x = 2.0f * RandNum0ToLessThan1();
        Sum += x*x;
    }
    
    double Width = 2.0;
    double Height = Sum / Count;
    cout.precision(dbl::max_digits10);
    cout << "i = " << Width * Height << std::endl;
    
#elif MCINT_NONUNIFORM
    /*
Calculate the integral of x^2 from the range [0,2] numerically using 
pdf p(r) = 0.5f * r and 
P-1(r) = sqrt(4*r)

 Now we need less samples!!

*/
    
    int Count = 700000;
    double Sum = 0.0f;
    for(int i = 0; i < Count; i++)
    {
        double x = sqrt(4.0 * double(RandNum0ToLessThan1()));
        Sum += x * x / pdf_nonuniform(x);
    }
    
    double Average = Sum / Count;
    cout.precision(dbl::max_digits10);
    cout << "i = " << Average << std::endl;
    
#elif MCINT_UNIFORM
    
    /*
    Calculate the integral of x^2 from the range [0,2] numerically using 
    pdf p(r) = 0.5f * r and 
    P-1(r) = sqrt(4*r)
    
     Now we need less samples!!
    */
    
    int Count = 1000000;
    double Sum = 0.0f;
    for(int i = 0; i < Count; i++)
    {
        double x = 2.0 * double(RandNum0ToLessThan1());
        Sum += x * x / pdf_uniform(x);
    }
    
    double Average = Sum / Count;
    cout.precision(dbl::max_digits10);
    cout << "i = " << Average << std::endl;
    
#elif MCINT_EXACT
    /*
    Calculate the integral of x^2 from the range [0,2] numerically using 
    pdf p(r) = 0.5f * r and 
    P-1(r) = sqrt(4*r)
    
     Now we need less samples!!
    */
    
    int Count = 1;
    double Sum = 0.0f;
    for(int i = 0; i < Count; i++)
    {
        double x = pow(double(RandNum0ToLessThan1()),1/3);
        Sum += x * x / pdf_exact(x);
    }
    
    double Average = Sum / Count;
    cout.precision(dbl::max_digits10);
    cout << "i = " << Average << std::endl;
    
#elif MCINT_SPHERE
    
    int Count = 1000;
    double Sum = 0.0f;
    for(int i = 0; i < Count; i++)
    {
        vec3 v = RandomInUnitSphere();
        double CS = v.z * v.z;
        Sum += CS / pdf_sphere(v);
    }
    
    double Average = Sum / Count;
    cout.precision(dbl::max_digits10);
    cout << "i = " << Average << std::endl;
    
#elif BERNOULLI_TRIAL
    
    uint64_t N = atoi(argv[1]);
    uint64_t NFac = Fact(N);
    cout.precision(dbl::max_digits10);
    for (uint64_t n =0 ;n <= N ; ++n){
        uint64_t C = NFac / (Fact(n) * Fact(N - n));
        double P = C * pow(0.5, n) * pow(0.5, N - n);
        std::cout << n << " " << P << std::endl;
    }
    
#elif EXP_DICE
    std::mt19937 rng;
    rng.seed(2013);
    std::uniform_int_distribution<uint32_t> die(1,6);
    int sum = 0;
    int samplecount = 1000;
    
    cout.precision(dbl::max_digits10);
    for(int n = 1; n < samplecount; n++)
    {
        int rnd = die(rng);
        sum += rnd;
        cout << n << " " << double(sum) / double(n) << std::endl;
    }
    
#elif SAMPLING_DISTRIBUTION
    
    /*
Results:

4 samples

Standard Error (theory) 2.936708
Standard Error (practice) 2.895162

16 samples

Standard Error (theory) 1.468354
Standard Error (practice) 1.443926

64 samples

Standard Error (theory) 0.734177
Standard Error (practice) 0.723249


100 samples 
pop size 395 pop mean 9.891139 pop variance 34.497010
Sampling Distribution Mean: 9.875776
Sampling Distribution Variance: 0.347418
Standard Error (theory) 0.587342
Standard Error (practice) 0.589422

10000 samples 
pop size 395 pop mean 9.891139 pop variance 34.497010
Sampling Distribution Mean: 9.891720
Sampling Distribution Variance: 0.003399
Standard Error (theory) 0.058734
Standard Error (practice) 0.058302

*/
    
    //int MinSamples = atoi(argv[1]);
    //int MaxSamples = atoi(argv[2]);
    
    std::mt19937 rng;
    uint32_t Buckets[BUCKETSIZE];
    int MeanDistr[BUCKETSIZE];
    uint32_t PopuSize = 0;
    double PopuMean = 0;
    double PopuVariance = 0;
    
    rng.seed(17);
    
    //creation Buckets
    std::uniform_int_distribution<uint32_t> distr(1, MAX_FREQ);
    for(int i = 0; i < BUCKETSIZE; ++i)
    {
        MeanDistr[i] = 0;
        Buckets[i] = distr(rng);
        PopuSize += Buckets[i];
        PopuMean += (double)Buckets[i] * (double)i; //Count * xi
        PopuVariance += (double)Buckets[i] * (double)i * (double)i;//Count * xi^2
    }
    
    //PopuMean has to be a value between 0 and 20
    PopuMean /= (double)PopuSize;
    PopuVariance /= (double)PopuSize;
    PopuVariance -= PopuMean * PopuMean;
    fprintf(stderr, "pop size %d pop mean %f pop variance %f\n", PopuSize, PopuMean, PopuVariance);
    
    //std::uniform_int_distribution<uint32_t> TestDistr(MinSamples, MaxSamples);
    std::uniform_int_distribution<uint32_t> SamplesDistr(0, PopuSize - 1);
    
    double EDistrMeans = 0;
    double EDistrVariances = 0;
    
    uint32_t SampleCount = atoi(argv[1]);
    const static uint32_t TestCount = 10000;
    for(uint32_t i = 0; i < TestCount; i++)
    {
        //uint32_t SampleCount = TestDistr(rng);
        //SMean has to be a vlue between 0 and 20
        double SMean = 0.0f, SVariance = 0.0f;
        for(uint32_t j = 0; j < SampleCount; j++)
        {
            int Item = SamplesDistr(rng);
            int k = 0;
            for(; k < BUCKETSIZE; ++k)
            {
                Item -= Buckets[k];
                if(Item < 0) break;
            }
            
            SMean += k;
            SVariance += k * k;
        }
        
        SMean /= (double)SampleCount;
        SVariance /= (double)SampleCount;
        SVariance -= SMean * SMean;
        
        EDistrMeans += SMean;
        EDistrVariances += SMean * SMean;
        
        int SMeanInt = (int)std::round(SMean);
        MeanDistr[SMeanInt]++;
        //printf("Test %d - SampleCount: %d, sample mean: %f, sample variance: %f\n", i, SampleCount, SMean, SVariance);
    }
    
    EDistrMeans /= TestCount;
    EDistrVariances /= TestCount;
    EDistrVariances -= EDistrMeans * EDistrMeans;
    
    printf("Sampling Distribution Mean: %f\n", EDistrMeans);
    printf("Sampling Distribution Variance: %f\n", EDistrVariances);
    printf("Standard Error (theory) %f\nStandard Error (practice) %f\n", std::sqrt(PopuVariance) / std::sqrt(SampleCount), std::sqrt(EDistrVariances));
    // NOTE(Alex): this follows a gaussian distribution!!!!
    //for(int i = 0; i < BUCKETSIZE; ++i)
    //{
    //printf("Discrete Mean %i, %i\n", i, MeanDistr[i]);
    //}
    
#elif PDF
    int NumIter = 10;
    float Minbound = -5.0f, MaxBound = 5.0f;
    float cdf = 0;
    float dx = (MaxBound - Minbound) / NumIter;
    for(int i = 0; i < NumIter; ++i)
    {
        float x = Minbound + dx * (i + 0.5f); 
        float gx = 1 / sqrtf(2.0f*(float)M_PI) * exp(-(x*x) / 2);
        cdf += gx * dx;
        printf("x: %f, g(x): %f, cdf: %f\n", x, gx, cdf);
    }
    
#elif TS_0
    /*
// NOTE(Alex): this is the shitty way, because the pdf is not a normla distribution
*/
    int SimCount = 10;
    float Dist = 10.0f; //10 km
    for(int i = 0; i < SimCount; i++)
    {
        float Time = 25 + RandNum0ToLessThan1() * 10;
        float Speed = 60.0f * Dist / Time;//Km per hour
        printf("Train Time: %f min, Speed: %f Km/h\n", Time, Speed);
    }
#elif TS_1
    /*
bring the probability distribution function 
// NOTE(Alex): the probability is uniform
print it an make sure the sum of probabilities = 1 
The random number generator used by the program should generate
random numbers with the same probability distribution function(or PDF) than the PDF
of the random process we try to simulate.

Difference - continuous random variable.


*/
    int SimCount = 1000000;
    int BinCount = 10;
    int Bins[10];
    memset(Bins, 0, sizeof(int) * BinCount);
    const float Dist = 10.0f;
    for(int i = 0; i < SimCount; i++)
    {
        // NOTE(Alex): My variable x goes from -5.0f to 5.0f
        //float x = 5.0f * (2.0f * RandNum0ToLessThan1() - 1.0f);
        float x = 10.0f * RandNum0ToLessThan1();
        // NOTE(Alex): I need to convert from [-5 , 5] to [0 , 10]
        int Index = (int)x;
        Bins[Index]++;
    }
    float Sum = 0.0f;
    for(int i = 0; i < BinCount; i++)
    {
        float p = (float)Bins[i] / (float)SimCount;
        Sum += p;
        printf("Bin %i probability %f\n", i, p);
    }
    
    printf("Sum = %f\n", Sum);
#elif CDF_DISCRETIZED
    
    //const int SampleCount = 10;
    /*
Man this is so cool!!!!!!!!!!!!!!! i can discretize a function with reimann sumns!!!
Soo what we try to do now is to use our cdf to get x places to sample. with. 
!!!!
*/
    
    // NOTE(Alex): We calculate the CDF!!!!
    
    cout.precision(flt::max_digits10);
    float MinBound = -5.0f, MaxBound = 5.0f;
    const int BinCount = 32;
    float CDF[BinCount];
    float X[BinCount];
    memset(CDF, 0, sizeof(float) * BinCount);
    float dx = (MaxBound - MinBound) / BinCount;
    float Sum = 0;
    // NOTE(Alex): We wanna make sure that the CDF
    //starts at 0.0f and ends at 1.0f
    CDF[0] = 0.0f;
    CDF[BinCount - 1] = 1.0f;
    for(int i = 1; i < (BinCount - 1); i++)
    {
        float x = MinBound + dx * (i + 0.5f * dx);
        float p = NormalDistributionPDF(x);
        Sum += p * dx;
        CDF[i] = Sum;
    }
    
    for(int i = 0; i < BinCount; i++)
    {
        X[i] = MinBound + dx * (i + 0.5f * dx);
        //printf("At %f, CDF[%i]: %f\n", X[i], i, CDF[i]);
    }
    
    // NOTE(Alex): Simulation
    
    int SimCount = 100000;
    const int HistCount = 100;
    int Hist[HistCount];
    memset(Hist, 0, sizeof(int) * HistCount);
    const float Dist = 10.0f;
    for(int i = 0; i < SimCount; i++)
    {
        float r = RandNum0ToLessThan1();
        float * ptr = std::lower_bound(CDF, CDF + BinCount, r);
        // NOTE(Alex): This difference is element wise not address wise
        size_t Temp = ptr-CDF;
        size_t LowerOffset = 0;
        if(Temp == 0)
        {
            LowerOffset = Temp;
        }
        else
        {
            LowerOffset = Temp - 1;
        }
        
        size_t UpperOffset = LowerOffset + 1;
        
        //printf("%zi, %zi\n", LowerOffset, UpperOffset); 
        //printf("Ys [%f, %f, %f]\n", CDF[LowerOffset], r, CDF[UpperOffset]); 
        /*
    Now we want to get the y value for r, between the two bins 
    */
        
        float t = (r - CDF[LowerOffset]) / (CDF[UpperOffset] - CDF[LowerOffset]);
        float ds = t * dx;
        float a =  X[LowerOffset] + ds;
        
        //printf("Xs [%f, %f, %f]\n", X[LowerOffset], a, X[UpperOffset]); 
        // NOTE(Alex): So a goes from -5.0f to 5.0f we need to convert it to 0 - HistCount
        float b = (a + 5.0f) / 10.0f;
        int Index = (int)(b * HistCount);
        Hist[Index]++;
    }
    
    Sum = 0.0f;
    for(int i = 0; i < HistCount; i++)
    {
        float p = (float)Hist[i] / (float)SimCount;
        Sum += p;
        printf("Bin %i probability %f\n", i, p);
    }
    
    printf("Sum = %f\n", Sum);
    
#endif
}

























