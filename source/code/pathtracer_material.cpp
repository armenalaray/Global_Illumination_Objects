//////////////////////////////////////////////////////
//
//Author: Alejandro Armenta
//Date: 2019
//File: pathtracer_material.cpp
//Company:
//
//(2018-2019) All rights reserved.
//
//////////////////////////////////////////////////////



vec3 Reflect(const vec3 & v, const vec3 & Normal)
{
	return v - (Dot(v, Normal) * 2 * Normal);
}

bool Refract(const vec3 & I_ ,const vec3 & N_, const float n1_, const float n2_, vec3 & OutT)
{
	vec3 I = MakeUnitVector(I_);
	vec3 N = MakeUnitVector(N_);
	float C1 = Dot(I, N);
	float n1 , n2 = 0.0f;
	
	if(C1 > 0)
	{
		// NOTE(Alex): We are inside 
		N = -N;
		n1 = n2_;
		n2 = n1_;
		
	}
	else
	{
		// NOTE(Alex): We are outside 
		C1 = -C1;
		n1 = n1_;
		n2 = n2_;
	}
	
	float NRatio = n1 / n2;
	float Radicand = (1 - NRatio * NRatio * (1 - C1 * C1));
	if(Radicand >= 0)
	{
		float C2 = sqrt(Radicand); 
		OutT = N * (NRatio * C1 - C2) + NRatio * I;
		return true;
	}
	else
	{
		// NOTE(Alex): Total internal reflection
		return false;
	}
}

float Fresnel(const vec3 I_, const vec3 N_, float N1_, float N2_)
{
	float Result = 0.0f;
	vec3 I = MakeUnitVector(I_);
	vec3 N = MakeUnitVector(N_);
	float N1, N2 = 0.0f;
	float CT1 = Dot(I, N);
	
	if(CT1 > 0)
	{
		// NOTE(Alex): We are inside 
		N = -N;
		N1 = N2_;
		N2 = N1_;
	}
	else
	{
		// NOTE(Alex): We are outside 
		CT1 = -CT1;
		N1 = N1_;
		N2 = N2_;
	}
	
	float ST2 = (N1 / N2) * sqrt(fmax(0.0f, 1 - CT1 * CT1));
	
	if(ST2 >= 1)
	{
		// NOTE(Alex): Total internal reflection;
		Result = 1.0f;
	}
	else
	{
		float CT2 = sqrt(fmax(0.0f, 1 - ST2 * ST2));
		float FR = (N2 * CT1  - N1 * CT2) / (N2 * CT1 + N1 * CT2);
		float FRP = (N1 * CT2  - N2 * CT1) / (N1 * CT2 + N2 * CT1);
		Result = 0.5f * (FR * FR + FRP * FRP);	
	}
	return Result;	
}


// TODO(Alex): Check if this function the cosine is in radians or degrees!
float Schlick(const vec3 I_, const vec3 N_, float N1_, float N2_)
{
	vec3 I = MakeUnitVector(I_);
	vec3 N = MakeUnitVector(N_);
	float Cosine;
	float IOR = N2_;
	float CT1 = Dot(I, N);
	if(CT1 > 0)
	{
		N = -N;
		Cosine = IOR * CT1;
	}
	else
	{
		Cosine = -CT1;
	}
	
	float R0 = (1 - IOR) / (1 + IOR);
	R0 = R0 * R0;
	return R0 + (1-R0)*pow((1-Cosine), 5);
}

float ShirleysSchlick(float cosine, float ref_idx)
{
	float r0 = (1-ref_idx) / (1+ref_idx);
	r0 = r0 * r0;
	return r0 + (1-r0)* pow((1-cosine), 5);
}

bool ShirleysRefract(const vec3 & v, const vec3 & n, float ni_over_nt, vec3 & refracted)
{
	vec3 uv = MakeUnitVector(v);
	float dt = Dot(uv, n);
	float discriminant = 1.0f - ni_over_nt*ni_over_nt*(1-dt*dt);
	if(discriminant > 0)
	{
		refracted = ni_over_nt*(uv - n*dt) - n*sqrt(discriminant);
		return true;
	}
	else
		return false;
	
}
