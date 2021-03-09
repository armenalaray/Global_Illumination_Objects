#ifndef PATHTRACER_TEXTURE_H
#define PATHTRACER_TEXTURE_H

class texture
{
	public:
	virtual vec3 Value(float u, float v, const vec3 p) = 0;
	virtual ~texture();
};

texture::~texture(){}

class constant_texture : public texture
{
	public:
	vec3 Color;
	constant_texture(vec3 & Color_) : Color(Color_) {}
	virtual vec3 Value(float u, float v, const vec3 p)
	{
		return Color;
	}
};

class uvw_texture : public texture
{
	public:
	vec3 uC, vC, wC;
	
	uvw_texture(vec3 uC_,vec3 vC_,vec3 wC_) : uC(uC_), vC(vC_), wC(wC_) {}
	
	virtual vec3 Value(float u, float v, const vec3 p)
	{
		float w = 1 - u - v;
		vec3 Color = u * uC +  v * vC + w * wC;
		return Color;
	}
};

class image_texture : public texture
{
	// NOTE(Alex): We are assuming 3 channels per pixel, top left image start.
	public:
	unsigned char * Data = 0;
	int Width;
	int Height;
	int ChannelsPPixel;
	
	image_texture(std::string FileFP) 
	{
		// TODO(Alex): Try catch block!!!
		unsigned char * Temp = stbi_load(FileFP.c_str(), &Width, &Height, &ChannelsPPixel, 0);
		if(Temp)
		{
			Data = Temp; 
		}
	}
	
	virtual vec3 Value(float u, float v, const vec3 p)
	{
		int i = (int)(u * (Width - 1));
		int j = (int)(v * (Height - 1));
		
		if(i > Width - 1) i = Width - 1;
		if(j > Height - 1) j = Height - 1;
		if(i < 0) i = 0;
		if(j < 0) j = 0;
		
		float R = (Data[(j * Width * 3) + (i * 3) + 0]) / 255.0f; 
		float G = (Data[(j * Width * 3) + (i * 3) + 1]) / 255.0f; 
		float B = (Data[(j * Width * 3) + (i * 3) + 2]) / 255.0f; 
		// NOTE(Alex): This is for gamma correction!
		return vec3(powf(R, 2),powf(G,2),powf(B,2));
	}
	
	~image_texture()
	{
		if(Data) stbi_image_free(Data);
	}
};


class image_texture_xy_z0 : public texture
{
	// NOTE(Alex): We are assuming 3 channels per pixel, top left image start.
	public:
	unsigned char * Data = 0;
	int Width;
	int Height;
	int ChannelsPPixel;
	
	image_texture_xy_z0(std::string FileFP) 
	{
        Data = stbi_load(FileFP.c_str(), &Width, &Height, &ChannelsPPixel, 0);
	}
	
    // NOTE(Alex): Data[0] = Top-Left most pixel with y scanlines of sixe x
	virtual vec3 Value(float u, float v, const vec3 p)
	{
        float tx = u;
        float ty = 1.0f-v;
        
		int i = (int)(tx * (Width - 1));
		int j = (int)(ty * (Height - 1));
		
		if(i > Width - 1) i = Width - 1;
		if(j > Height - 1) j = Height - 1;
		if(i < 0) i = 0;
		if(j < 0) j = 0;
		
		float R = (Data[(j * Width * 3) + (i * 3) + 0]) / 255.0f; 
		float G = (Data[(j * Width * 3) + (i * 3) + 1]) / 255.0f; 
		float B = (Data[(j * Width * 3) + (i * 3) + 2]) / 255.0f; 
		// NOTE(Alex): This is for gamma correction!
		return vec3(powf(R, 2),powf(G,2),powf(B,2));
	}
	
	~image_texture_xy_z0()
	{
		if(Data) stbi_image_free(Data);
	}
};


class image_texture_yz_x0 : public texture
{
	// NOTE(Alex): We are assuming 3 channels per pixel, top left image start.
	public:
	unsigned char * Data = 0;
	int Width;
	int Height;
	int ChannelsPPixel;
	
	image_texture_yz_x0(std::string FileFP) 
	{
        Data = stbi_load(FileFP.c_str(), &Width, &Height, &ChannelsPPixel, 0);
	}
	
	virtual vec3 Value(float u, float v, const vec3 p)
	{
        float tz = 1.0f - u;
        float ty = 1.0f - v;
        
		int i = (int)(tz * (Width - 1));
		int j = (int)(ty * (Height - 1));
		
		if(i > Width - 1) i = Width - 1;
		if(j > Height - 1) j = Height - 1;
		if(i < 0) i = 0;
		if(j < 0) j = 0;
		
		float R = (Data[(j * Width * 3) + (i * 3) + 0]) / 255.0f; 
		float G = (Data[(j * Width * 3) + (i * 3) + 1]) / 255.0f; 
		float B = (Data[(j * Width * 3) + (i * 3) + 2]) / 255.0f; 
		// NOTE(Alex): This is for gamma correction!
		return vec3(powf(R, 2),powf(G,2),powf(B,2));
	}
	
	~image_texture_yz_x0()
	{
		if(Data) stbi_image_free(Data);
	}
};


class image_texture_xz_y0 : public texture
{
	// NOTE(Alex): We are assuming 3 channels per pixel, top left image start.
	public:
	unsigned char * Data = 0;
	int Width;
	int Height;
	int ChannelsPPixel;
	
	image_texture_xz_y0(std::string FileFP) 
	{
        Data = stbi_load(FileFP.c_str(), &Width, &Height, &ChannelsPPixel, 0);
	}
	
	virtual vec3 Value(float u, float v, const vec3 p)
	{
        float tz = 1.0f - u;
        float tx = v;
        
		int i = (int)(tz * (Width - 1));
		int j = (int)(tx * (Height - 1));
		
		if(i > Width - 1) i = Width - 1;
		if(j > Height - 1) j = Height - 1;
		if(i < 0) i = 0;
		if(j < 0) j = 0;
		
		float R = (Data[(j * Width * 3) + (i * 3) + 0]) / 255.0f; 
		float G = (Data[(j * Width * 3) + (i * 3) + 1]) / 255.0f; 
		float B = (Data[(j * Width * 3) + (i * 3) + 2]) / 255.0f; 
		// NOTE(Alex): This is for gamma correction!
		return vec3(powf(R, 2),powf(G,2),powf(B,2));
	}
	
	~image_texture_xz_y0()
	{
		if(Data) stbi_image_free(Data);
	}
};

class image_texture_yz_x5 : public texture
{
	// NOTE(Alex): We are assuming 3 channels per pixel, top left image start.
	public:
	unsigned char * Data = 0;
	int Width;
	int Height;
	int ChannelsPPixel;
	
	image_texture_yz_x5(std::string FileFP) 
	{
        Data = stbi_load(FileFP.c_str(), &Width, &Height, &ChannelsPPixel, 0);
	}
	
	virtual vec3 Value(float u, float v, const vec3 p)
	{
        float tz = u;
        float ty = 1.0f - v;
        
		int i = (int)(tz * (Width - 1));
		int j = (int)(ty * (Height - 1));
		
		if(i > Width - 1) i = Width - 1;
		if(j > Height - 1) j = Height - 1;
		if(i < 0) i = 0;
		if(j < 0) j = 0;
		
		float R = (Data[(j * Width * 3) + (i * 3) + 0]) / 255.0f; 
		float G = (Data[(j * Width * 3) + (i * 3) + 1]) / 255.0f; 
		float B = (Data[(j * Width * 3) + (i * 3) + 2]) / 255.0f; 
		// NOTE(Alex): This is for gamma correction!
		return vec3(powf(R, 2),powf(G,2),powf(B,2));
	}
	
	~image_texture_yz_x5()
	{
		if(Data) stbi_image_free(Data);
	}
};


class image_texture_xz_y5 : public texture
{
	// NOTE(Alex): We are assuming 3 channels per pixel, top left image start.
	public:
	unsigned char * Data = 0;
	int Width;
	int Height;
	int ChannelsPPixel;
	
	image_texture_xz_y5(std::string FileFP) 
	{
		Data = stbi_load(FileFP.c_str(), &Width, &Height, &ChannelsPPixel, 0);
	}
	
	virtual vec3 Value(float u, float v, const vec3 p)
	{
        float tz = 1.0f - u;
        float tx = v;
        
		int i = (int)(tz * (Width - 1));
		int j = (int)(tx * (Height - 1));
		
		if(i > Width - 1) i = Width - 1;
		if(j > Height - 1) j = Height - 1;
		if(i < 0) i = 0;
		if(j < 0) j = 0;
		
		float R = (Data[(j * Width * 3) + (i * 3) + 0]) / 255.0f; 
		float G = (Data[(j * Width * 3) + (i * 3) + 1]) / 255.0f; 
		float B = (Data[(j * Width * 3) + (i * 3) + 2]) / 255.0f; 
		// NOTE(Alex): This is for gamma correction!
		return vec3(powf(R, 2),powf(G,2),powf(B,2));
	}
	
	~image_texture_xz_y5()
	{
		if(Data) stbi_image_free(Data);
	}
};

class checker_texture : public texture
{
	public:
	shared_ptr<texture> Odd;
	shared_ptr<texture> Even;
	
	checker_texture(shared_ptr<texture> Odd_,shared_ptr<texture> Even_) : Odd(Odd_), Even(Even_) {}
	virtual vec3 Value(float u, float v, const vec3 p)
	{
		vec3 Result;
		float t = sin(10.0f * p.X()) * sin(10.0f * p.Z()) * sin(10.0f * p.Y());
		if(t < 0)
		{
			Result = Odd->Value(u,v,p); 
		}
		else
		{
			Result = Even->Value(u,v,p); 			
		}
		return Result;
	}
};

template<typename T>
class noise_texture : public texture
{
	public:
	T NoiseType;
	vec3 Color;
	noise_texture(vec3 & Color_) : NoiseType() , Color(Color_) {}
	virtual vec3 Value(float u, float v, const vec3 p) 
	{
		vec3 a = p * 8.0f;
		return Color * NoiseType.Eval3D(a);
	}
};


template<typename T>
class wood_texture : public texture
{
	float MaxValue;
    
	public:
	T Noise;
	int LayerCount;
	float InitialFrequency;
	float Lacunarity;
	float Gain;	
	vec3 HighColor;
	vec3 LowColor;
	
	wood_texture(vec3 & LowColor_, vec3 & HighColor_, float InitialFrequency_, float Lacunarity_,  float Gain_, int LayerCount_)  : 
	Noise(), 
	LowColor(LowColor_), 
	HighColor(HighColor_), 
	LayerCount(LayerCount_), 
	Lacunarity(Lacunarity_), 
	InitialFrequency(InitialFrequency_), 
	Gain(Gain_) 
	{
	}
	
	virtual vec3 Value(float u, float v, const vec3 p) 
	{
		float Value = Wood(Noise, p, InitialFrequency, Lacunarity, Gain, LayerCount);
		return Lerp<vec3>(LowColor, HighColor, Value);
	}
};


template<typename T>
class marble_texture : public texture
{
	float MaxValue;
    
	public:
	T Noise;
	int LayerCount;
	float InitialFrequency;
	float Lacunarity;
	float Gain;	
	vec3 Color;
	
	marble_texture(vec3 & Color_, float InitialFrequency_, float Lacunarity_,  float Gain_, int LayerCount_)  : 
	Noise(), 
	Color(Color_), 
	LayerCount(LayerCount_), 
	Lacunarity(Lacunarity_), 
	InitialFrequency(InitialFrequency_), 
	Gain(Gain_) 
	{
	}
	
	virtual vec3 Value(float u, float v, const vec3 p) 
	{
		float Value = Marble(Noise, p, InitialFrequency, Lacunarity, Gain, LayerCount);
		return Color * Value;
	}
};


#endif

