//////////////////////////////////////////////////////
//
//Author: Alejandro Armenta
//Date: 2019
//File: volume_test.cpp
//Company:
//
//(2018-2019) All rights reserved.
//
//////////////////////////////////////////////////////

#include "pathtracer.h"

int main()
{
	int Width = 256;
	int Height = 256;
	
	// NOTE(Alex): 20 units
	float VolumeLength = 20.0f;
	float SigmaA = 0.0f;
	
	
	vec3 * Buffer = new vec3[Width * Height];
	vec3 * Transmittance = Buffer + (Width - 1);
	
	for(int i = 0; i < Width; i++)
	{
		float l = ((float)i / (float)(Width - 1)) * VolumeLength;
		float Value = exp(-SigmaA * l);
		*Transmittance-- = vec3(Value, Value, Value);
	}
	
	for(int i = 1; i < Height; i++)
	{
		memcpy(Buffer + i * Width, Buffer, sizeof(vec3) * Width);
	}
	
	ofstream ofs("volume.ppm");
	ofs << "P3\n" << Width << " " << Height << "\n255\n";
	
	for(int j = Height -1 ; j >= 0; --j)
	{
		for(int i = 0 ; i < Width; ++i)
		{
			vec3 & Col = Buffer[j * Width + i];
			//vec3 Col(sqrt(p), sqrt(p), sqrt(p));
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
	
	delete [] Buffer;
	
}