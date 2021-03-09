
#include "pathtracer.h"

#if 0
#define _USE_MATH_DEFINES 
#include <cmath> 
#include <cstdio> 
#include <random> 
#include <functional> 
#include <iostream> 
#include <fstream> 
#include <algorithm> 

using namespace std;


template<typename T> 
class vec2 
{ 
	public: 
    vec2() : x(T(0)), y(T(0)) {} 
    vec2(T xx, T yy) : x(xx), y(yy) {} 
    vec2 operator * (const T &r) const { return vec2(x * r, y * r); } 
    vec2& operator *= (const T &r) { x *= r, y *= r; return *this; } 
    T x, y; 
}; 


template<typename T> 
class vec3 
{ 
	public: 
    vec3() : x(T(0)), y(T(0)), z(T(0)) {} 
    vec3(T xx, T yy, T zz) : x(xx), y(yy), z(zz) {} 
    vec3 operator * (const T &r) const { return vec3(x * r, y * r, z * r); } 
    vec3 operator - (const vec3 &v) const { return vec3(x - v.x, y - v.y, z - v.z); } 
    vec3& operator *= (const T &r) { x *= r, y *= r, z *= r; return *this; } 
    T length2() const { return x * x + y * y + z * z; } 
    vec3& operator /= (const T &r) { x /= r, y /= r, z /= r; return *this; } 
    vec3 cross(const vec3 &v) const 
    { 
        return vec3( 
            y * v.z - z * v.y, 
            z * v.x - x * v.z, 
            x * v.y - y * v.x 
			); 
    } 
    vec3& normalize() 
    { 
        T len2 = length2(); 
        if (len2 > 0) { 
            T invLen = T(1) / sqrt(len2); 
            x *= invLen, y *= invLen, z *= invLen; 
        } 
        return *this; 
    } 
    friend std::ostream & operator << (std::ostream &os, const vec3<T> &v) 
    { 
        os << v.x << ", " << v.y << ", " << v.z; 
        return os; 
    } 
    T x, y, z; 
}; 

typedef vec2<float> vec2f; 
typedef vec3<float> vec3f; 

template<typename T = float> 
inline T Dot(const vec3<T> &a, const vec3<T> &b) 
{ return a.x * b.x + a.y * b.y + a.z * b.z; } 


template<typename T = float> 
inline T Lerp(const T &lo, const T &hi, const T &t) 
{ 
    return lo * (1 - t) + hi * t; 
}

#endif


static float SmoothFunction(float t)
{
	return t * t * (3 - 2 * t);
}

static float CosineFunction(float t)
{
	return (1.0f - cosf(t * (float)M_PI)) * 0.5f;
}


#if 0
static int 
RoundToMinusInf(float x)
{
	int Result = (int)x - (x < 0 && x != (int)x);
	return Result;
}
#endif


class perlin_noise 
{
	public:
	static const unsigned VertexCount = 256; 
    static const unsigned VertexMask = 0xFF; 
    vec3 Lattice[VertexCount]; 
    int permutationTable[VertexCount * 2]; 
	
	perlin_noise(const unsigned &seed = 2016) 
    { 
        std::mt19937 generator(seed); 
        std::uniform_real_distribution<float> distribution; 
        auto dice = std::bind(distribution, generator); 
        for (unsigned i = 0; i < VertexCount; ++i) { 
#if 0 
            // bad
            float gradientLen2; 
            do { 
                gradients[i] = vec3(2 * dice() - 1, 2 * dice() - 1, 2 * dice() - 1); 
                gradientLen2 = gradients[i].length2(); 
            } while (gradientLen2 > 1); 
            gradients[i].normalize(); 
#else 
            // better
            float theta = acos(2 * dice() - 1); 
            float phi = 2 * dice() * (float)M_PI; 
			
            float x = cos(phi) * sin(theta); 
            float y = sin(phi) * sin(theta); 
            float z = cos(theta); 
            Lattice[i] = vec3(x, y, z); 
#endif 
            permutationTable[i] = i; 
        } 
		
        std::uniform_int_distribution<unsigned> distributionInt; 
        auto diceInt = std::bind(distributionInt, generator); 
        // create permutation table
        for (unsigned i = 0; i < VertexCount; ++i) 
            std::swap(permutationTable[i], permutationTable[diceInt() & VertexMask]); 
        // extend the permutation table in the index range [256:512]
        for (unsigned i = 0; i < VertexCount; ++i) { 
            permutationTable[VertexCount + i] = permutationTable[i]; 
        } 
    } 
	
    virtual ~perlin_noise() {} 
	
	
	vec3 & Hash3D(const int &x, const int &y, const int &z) 
    { 
        return Lattice[permutationTable[permutationTable[permutationTable[x] + y] + z]]; 
    } 

#if 0	
	vec2f & Hash2D(const int &x, const int &y, const int &z) 
    { 
        return Lattice[permutationTable[permutationTable[x] + y]]; 
    } 
	
	float Eval2D(float x, float y)
	{
		float Result = 0.0f;
		int xi0, xi1, yi0, yi1;
		xi0 = RoundToMinusInf(x);
		yi0 = RoundToMinusInf(y);
		
		xi1 = (xi0 + 1);
		yi1 = (yi0 + 1);
		
		float tx = x - (float)xi0;		
		float ty = y - (float)yi0;
		
		float u = SmoothFunction(tx); 
		float v = SmoothFunction(ty); 
		
		xi0 &= VertexMask;
		xi1 &= VertexMask;
		yi0 &= VertexMask;
		yi1 &= VertexMask;
		
		vec2f &c00 = Hash2D(xi0, yi0);
		vec2f &c01 = Hash2D(xi0, yi1);
		vec2f &c10 = Hash2D(xi1, yi0);
		vec2f &c11 = Hash2D(xi1, yi1);
		
		float x0 = tx;
		float y0 = ty;	
		float x1 = tx - 1;
		float y1 = ty - 1;
		
		vec2f p00 = vec2f(x0, y0);
		vec2f p01 = vec2f(x0, y1);
		vec2f p10 = vec2f(x1, y0);
		vec2f p11 = vec2f(x1, y1);
		
		float sx0 = Lerp(Dot(c00, p00), Dot(c10, p10), u);
		float sx1 = Lerp(Dot(c01, p01), Dot(c11, p11), u);
		
		Result = Lerp(sx0, sx1, v);
		return Result;
	}
#endif

	float Eval3D(vec3 p)
	{
		float x = p.x; 
		float y = p.y; 
		float z = p.z; 

		float Result = 0.0f;
		int xi0 = ((int)std::floor(p.x)) & VertexMask; 
        int yi0 = ((int)std::floor(p.y)) & VertexMask; 
        int zi0 = ((int)std::floor(p.z)) & VertexMask; 
		
        int xi1 = (xi0 + 1) & VertexMask; 
        int yi1 = (yi0 + 1) & VertexMask; 
        int zi1 = (zi0 + 1) & VertexMask; 
		
        float tx = p.x - ((int)std::floor(p.x)); 
        float ty = p.y - ((int)std::floor(p.y)); 
        float tz = p.z - ((int)std::floor(p.z)); 
		
		float u = SmoothFunction(tx); 
		float v = SmoothFunction(ty); 
		float w = SmoothFunction(tz); 
		
		xi0 &= VertexMask;
		xi1 &= VertexMask;
		yi0 &= VertexMask;
		yi1 &= VertexMask;
		zi0 &= VertexMask;
		zi1 &= VertexMask;
		
		vec3 &c000 = Hash3D(xi0, yi0, zi0);
		vec3 &c001 = Hash3D(xi0, yi0, zi1);
		vec3 &c010 = Hash3D(xi0, yi1, zi0);
		vec3 &c011 = Hash3D(xi0, yi1, zi1);
		vec3 &c100 = Hash3D(xi1, yi0, zi0);
		vec3 &c101 = Hash3D(xi1, yi0, zi1);
		vec3 &c110 = Hash3D(xi1, yi1, zi0);
		vec3 &c111 = Hash3D(xi1, yi1, zi1);

		float x0 = tx;
		float y0 = ty;
		float z0 = tz;
		
		float x1 = tx - 1;
		float y1 = ty - 1;
		float z1 = tz - 1;
				
		vec3 p000 = vec3(x0, y0, z0);
		vec3 p001 = vec3(x0, y0, z1);
		vec3 p010 = vec3(x0, y1, z0);
		vec3 p011 = vec3(x0, y1, z1);
		vec3 p100 = vec3(x1, y0, z0);
		vec3 p101 = vec3(x1, y0, z1);
		vec3 p110 = vec3(x1, y1, z0);
		vec3 p111 = vec3(x1, y1, z1);
		
		float sx00 = Lerp(Dot(c000, p000), Dot(c100, p100), u);
		float sx10 = Lerp(Dot(c010, p010), Dot(c110, p110), u);
		float sx01 = Lerp(Dot(c001, p001), Dot(c101, p101), u);
		float sx11 = Lerp(Dot(c011, p011), Dot(c111, p111), u);
		
		float sy0 = Lerp(sx00, sx10, v);
		float sy1 = Lerp(sx01, sx11, v);

		Result = Lerp(sy0, sy1, w);
		return Result;
	}

};

class value_noise
{
	public:
	// NOTE(Alex): these will only work with power of two
	static const int VertexMask = 0xFF;
	static const int VertexCount = 256;
	float Lattice[VertexCount];
	int PermTable[2*VertexCount];
	
	value_noise(unsigned seed = 2016) 
	{
		std::mt19937 Gen(seed);
		uniform_real_distribution<float> DistrFloat;
		auto RandFloat = bind(DistrFloat, Gen);
		
		for(int j = 0; j < VertexCount; j++)
		{
			Lattice[j] = RandFloat();
			PermTable[j] = j;
		}
		uniform_int_distribution<unsigned int> DistrInt;
		auto RandUint = bind(DistrInt, Gen);
		
		for(int j = 0; j < VertexCount; j++)
		{
			int a = RandUint() & VertexMask;
			swap(PermTable[j], PermTable[a]);
			PermTable[j + VertexCount] = PermTable[j];
		}
	}

#if 1	
	float Eval2D(float x, float y)
	{
		float Result = 0.0f;
		int x0, x1, y0, y1;
		x0 = RoundToMinusInf(x);
		y0 = RoundToMinusInf(y);
		x1 = (x0 + 1);
		y1 = (y0 + 1);
		
		float tx = x - (float)x0;		
		float ty = y - (float)y0;
		
		tx = SmoothFunction(tx); 
		ty = SmoothFunction(ty); 
		
		x0 &= VertexMask;
		x1 &= VertexMask;
		y0 &= VertexMask;
		y1 &= VertexMask;
		
		float a, b, c, d, s0,s1;
		a = Lattice[PermTable[PermTable[x0] + y0]];
		b = Lattice[PermTable[PermTable[x1] + y0]];
		c = Lattice[PermTable[PermTable[x0] + y1]];
		d = Lattice[PermTable[PermTable[x1] + y1]];
				
		s0 = Lerp(a,b,tx);		
		s1 = Lerp(c,d,tx);		
		Result = Lerp(s0,s1,ty);		
		return Result;
	}	

	
	float Eval1D(float x)
	{
		float Result = 0.0f;
		int x0, x1;
		x0 = RoundToMinusInf(x);
		x1 = (x0 + 1);
		float tx = x - (float)x0;
		
		tx = SmoothFunction(tx);
		
		x0 &= VertexMask;
		x1 &= VertexMask;
		
		float a, b;
		a = Lattice[x0];
		b = Lattice[x1];
		Result = Lerp(a,b,tx);		
		return Result;
	}	
	#endif
};

static value_noise VN;
static perlin_noise PN;



//////////////////////////////////////////////////////////////////////////////////////////////////
// NOTE(Alex):  NOISE Functions!!!!
//////////////////////////////////////////////////////////////////////////////////////////////////



typedef float (*NoiseFunc)(vec2f p, float InitialFrequency, float Lacunarity, float Gain, int LayerCount);

#if 0
float PerlinNoise(vec3 p, float InitialFrequency, float Lacunarity, float Gain, int LayerCount = 1)
{
	float Result = 0;
	p *= InitialFrequency;
	Result = (PN.Eval3D(p[0], 0, p[1]) + 1.0f) * 0.5f;
	return Result;
}
#endif

float FractalNoise(vec2f p, float InitialFrequency, float Lacunarity, float Gain, int LayerCount = 1)
{
	float Result = 0;
	float Amplitude = 1.0f;
	p *= InitialFrequency;
	for(int k = 0; k < LayerCount; ++k)
	{
		Result += VN.Eval2D(p.x, p.y) * Amplitude;
		p *= Lacunarity;
		Amplitude *= Gain;
	}
	return Result;
	
}

float TurbulenceNoise(vec2f p, float InitialFrequency, float Lacunarity, float Gain, int LayerCount = 1)
{
	float Result = 0;
	float Amplitude = 1.0f;
	p *= InitialFrequency;
	for(int k = 0; k < LayerCount; ++k)
	{
		Result += fabs((2 * VN.Eval2D(p.x, p.y)) - 1.0f) * Amplitude;
		p *= Lacunarity;
		Amplitude *= Gain;
	}
	return Result;
}

float Marble(vec2f p, float InitialFrequency, float Lacunarity, float Gain, int LayerCount = 1)
{
	float NoiseValue = FractalNoise(p, InitialFrequency, Lacunarity, Gain, LayerCount);
	//float Result = (sin((p.x + NoiseValue * 100) * 2 * (float)M_PI / 200.0f) + 1.0f) / 2.0f;
	//float Result = (sin((NoiseValue * 100.0f) + (p.x * 4 * M_PI / 200.0f)) + 1.0f) / 2.0f;
	//float Result = (sin((NoiseValue * 1.0f * p.x * p.x) * 2 * (float)M_PI / 200.0f) + 1.0f) / 2.0f;
	//float Result = (sin((NoiseValue * p.x) * 0.1f) + 1.0f) / 2.0f;
	//float Result = (sin((NoiseValue * 100.0f + p.x) * 0.1f) + 1.0f) / 2.0f;
	float Result = (sin((NoiseValue * 200.0f + p.x) * 0.1f) + 1.0f) / 2.0f;
	
	// NOTE(Alex): NICE square is periodic upon some limit which makes elements to jump to only positive values.
	//float Result = (sin((p.x * p.x) * 0.01f) + 1.0f) / 2.0f;
	return Result;
}


float Wood(vec2f p, float InitialFrequency, float Lacunarity, float Gain, int LayerCount = 1)
{
	float Amplitude = 10.0f;
	
#if 1	
	p *= InitialFrequency;
	float g = VN.Eval2D(p.x, p.y) * Amplitude;
#else
	float g = FractalNoise(p, InitialFrequency, Lacunarity, Gain, LayerCount) * Amplitude;	
#endif

	float Result = g - int(g);
	return Result;
}

void WriteImageToFile(char * FileName, float* Image, int Width, int Height)
{
	ofstream ofs(FileName);
	ofs << "P3\n" << Width << " " << Height << "\n255\n";
	for(int j = Height -1 ; j >= 0; --j)
	{
		for(int i = 0 ; i < Width; ++i)
		{
			float & p = Image[j * Width + i];
			vec3 Col(sqrt(p), sqrt(p), sqrt(p));
			ofs 
				<< (int)(Col.x * 255.99f) 
				<< " "
				<< (int)(Col.y * 255.99f) 
				<< " "
				<< (int)(Col.z * 255.99f) 
				<< "\n";
		}
	}	
	ofs.close();
}

template<NoiseFunc N>
void CreateNoiseImage(char * FileName, float InitialFrequency, float Lacunarity, float Gain, int LayerCount = 1)
{
	int Width, Height;
	Width = Height = 512;
	float * Image = new float[Width * Height];
	float MaxP = FLT_MIN;
	
	for(int j = Height -1 ; j >= 0; --j)
	{
		for(int i = 0 ; i < Width; ++i)
		{
			vec2f Pos((float)i, (float)j);
			float & p = Image[j * Width + i];
			p = (*N)(Pos, InitialFrequency, Lacunarity, Gain, LayerCount);
			if(p > MaxP) MaxP = p;
		}
	}
	
	for(int j = Height -1 ; j >= 0; --j)
	{
		for(int i = 0 ; i < Width; ++i)
		{
			float & p = Image[j * Width + i];
			p = p / MaxP;
		}
	}	
	
	WriteImageToFile(FileName, Image, Width, Height);
	delete [] Image;
}

int main()
{	
	const uint32_t width = 512, height = 512; 
    float *noiseMap = new float[width * height]; 
	
    for (uint32_t j = 0; j < height; ++j) { 
        for (uint32_t i = 0; i < width; ++i) { 
            noiseMap[j * width + i] = (PN.Eval3D(vec3((float)i, 0, (float)j) * (1 / 64.)) + 1) * 0.5f; 
        } 
    } 

	std::ofstream ofs; 
    ofs.open("perlin_noise.ppm", std::ios::out | std::ios::binary); 
    ofs << "P6\n" << width << " " << height << "\n255\n"; 
    for (unsigned k = 0; k < width * height; ++k) { 
        unsigned char n = static_cast<unsigned char>(noiseMap[k] * 255); 
        ofs << n << n << n; 
    } 
    ofs.close(); 
	
    delete[] noiseMap; 
	
}