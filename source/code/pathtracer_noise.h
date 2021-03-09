//////////////////////////////////////////////////////
//
//Author: Alejandro Armenta
//Date: 2019
//File: pathtracer_noise.h
//Company:
//
//(2018-2019) All rights reserved.
//
//////////////////////////////////////////////////////


#ifndef PATHTRACER_NOISE_H
#define PATHTRACER_NOISE_H

typedef float (*NoiseFunc)(vec3 p);

static float SmoothFunction(float t)
{
	return t * t * (3 - 2 * t);
}

static float CosineFunction(float t)
{
	return (1.0f - cosf(t * (float)M_PI)) * 0.5f;
}

class noise
{
	public:
	virtual float Eval3D(vec3 p)  = 0;
};

class perlin_noise : public noise 
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
        for (unsigned i = 0; i < VertexCount; ++i) 
		{ 
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
	
	
	vec3 & Hash3D(const int &x, const int &y, const int &z) 
    { 
        return Lattice[permutationTable[permutationTable[permutationTable[x] + y] + z]]; 
    } 
    
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
		return ((Result + 1.0f) * 0.5f);
	}
};

class value_noise : public noise
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
    
	float Hash3D(const int &x, const int &y, const int &z) 
    { 
        return Lattice[PermTable[PermTable[PermTable[x] + y] + z]]; 
    } 
	
	float Eval3D(vec3 p)
	{
		float Result = 0.0f;
		
		float x = p.x;
		float y = p.y;
		float z = p.z;
		
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
		
		float c000 = Hash3D(xi0, yi0, zi0);
		float c001 = Hash3D(xi0, yi0, zi1);
		float c010 = Hash3D(xi0, yi1, zi0);
		float c011 = Hash3D(xi0, yi1, zi1);
		float c100 = Hash3D(xi1, yi0, zi0);
		float c101 = Hash3D(xi1, yi0, zi1);
		float c110 = Hash3D(xi1, yi1, zi0);
		float c111 = Hash3D(xi1, yi1, zi1);
        
		float sx00 = Lerp(c000, c100, u);
		float sx10 = Lerp(c010, c110, u);
		float sx01 = Lerp(c001, c101, u);
		float sx11 = Lerp(c011, c111, u);
		
		float sy0 = Lerp(sx00, sx10, v);
		float sy1 = Lerp(sx01, sx11, v);
        
		Result = Lerp(sy0, sy1, w);
		return Result;
	}	
	
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
};


//static value_noise VN;
//static perlin_noise PN;


//////////////////////////////////////////////////////////////////////////////////////////////////
// NOTE(Alex):  NOISE Functions!!!!
//////////////////////////////////////////////////////////////////////////////////////////////////


template<typename T>
float FractalNoise(T Noise, vec3 p, float InitialFrequency, float Lacunarity, float Gain, int LayerCount = 1)
{
	float Result = 0;
	float Amplitude = 1.0f;
	p *= InitialFrequency;
	
	for(int k = 0; k < LayerCount; ++k)
	{
		Result += Noise.Eval3D(p) * Amplitude;
		p *= Lacunarity;
		Amplitude *= Gain;
	}
	return Result;
}

template<typename T>
float TurbulenceNoise(T Noise, vec3 p, float InitialFrequency, float Lacunarity, float Gain, int LayerCount = 1)
{
	float Result = 0;
	float Amplitude = 1.0f;
	p *= InitialFrequency;
	for(int k = 0; k < LayerCount; ++k)
	{
		Result += fabs((2 * Noise.Eval3D(p)) - 1.0f) * Amplitude;
		p *= Lacunarity;
		Amplitude *= Gain;
	}
	return Result;
}

template<typename T>
float Marble(T Noise, vec3 p, float InitialFrequency, float Lacunarity, float Gain, int LayerCount = 1)
{
	float NoiseValue = FractalNoise(Noise, p, InitialFrequency, Lacunarity, Gain, LayerCount);
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

template<typename T>
float Wood(T Noise, vec3 p, float InitialFrequency, float Lacunarity, float Gain, int LayerCount = 1)
{
	float Amplitude = 30.0f;
	
#if 1	
	p *= InitialFrequency;
	float g = Noise.Eval3D(p) * Amplitude;
#else
	float g = FractalNoise(p, InitialFrequency, Lacunarity, Gain, LayerCount) * Amplitude;	
#endif
	
	float Result = g - int(g);
	return Result;
}


#endif


