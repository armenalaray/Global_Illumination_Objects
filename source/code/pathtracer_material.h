//////////////////////////////////////////////////////
//
//Author: Alejandro Armenta
//Date: 2019
//File: pathtracer_material.h
//Company:
//
//(2018-2019) All rights reserved.
//
//////////////////////////////////////////////////////


#ifndef PATHTRACER_MATERIAL_H
#define PATHTRACER_MATERIAL_H


class material
{
	public:
	virtual bool Scatter(const ray& RayIn, const hit_record& Rec, vec3& attenuation, ray& Scattered) const = 0;
	virtual vec3 Emit(float u, float v, vec3 p)
	{
		return vec3(0,0,0);
	}
	virtual ~material();
};

material::~material(){}

class isotropic : public material
{
	public:
	shared_ptr<texture> Albedo;
	isotropic(shared_ptr<texture> Albedo_) : Albedo(Albedo_) {};
	virtual bool Scatter(const ray& RayIn, const hit_record& Rec, vec3& Attenuation, ray& Scattered) const
	{
		Scattered = ray(Rec.p, RandomInUnitSphere());
		Attenuation = Albedo->Value(Rec.u, Rec.v, Rec.p);
		return true;
	}
};

class lambertian : public material
{
	public:
	shared_ptr<texture> albedo;
	lambertian(shared_ptr<texture> albedo_) : albedo(albedo_) {}
	~lambertian() {};
	virtual bool Scatter(const ray& RayIn, const hit_record& Rec, vec3& attenuation, ray& Scattered) const
	{
		vec3 p = Rec.p + Rec.Normal + RandomInUnitSphere();
		Scattered = ray(Rec.p, p - Rec.p, RayIn.Time);
		attenuation = albedo->Value(Rec.u,Rec.v,Rec.p);
		return true;
	}
};


class metal : public material
{
	public:
	vec3 albedo;
	float Fuzz;
	
	metal(const vec3& albedo_, const float f ) : albedo(albedo_), Fuzz(f) {}
	~metal(){};
	virtual bool Scatter(const ray& RayIn, const hit_record& Rec, vec3& attenuation, ray& Scattered) const
	{
		vec3 p = Reflect(MakeUnitVector(RayIn.Direction), Rec.Normal); 
		Scattered = ray(Rec.p, p + Fuzz * RandomInUnitSphere(), RayIn.Time);
		attenuation = albedo;
		
		float d = Dot(Scattered.Direction, Rec.Normal); 
		return (d > 0);
	}
};

class dielectric : public material
{
	public:
	float ior;
	
	dielectric(const float ior_) : ior(ior_) {}
	~dielectric(){}
	virtual bool Scatter(const ray& RayIn, const hit_record& Rec, vec3& Attenuation, ray& Scattered) const
	{
		
#if 0
		vec3 outward_normal;
		vec3 reflected = Reflect(RayIn.Direction, Rec.Normal);
		
		float ni_over_nt;
		Attenuation = vec3(1.0f,1.0f,1.0f);
		
		vec3 refracted;
		float reflect_prob;
		float cosine;
		
		if(Dot(RayIn.Direction, Rec.Normal) > 0)
		{
			
			outward_normal = -Rec.Normal;
			ni_over_nt = ior;
			cosine = ior * Dot(RayIn.Direction, Rec.Normal) / RayIn.Direction.Length();
		}
		else
		{
			
			outward_normal = Rec.Normal;
			ni_over_nt = 1.0f / ior;
			cosine = -Dot(RayIn.Direction, Rec.Normal) / RayIn.Direction.Length();
		}
		
		if(ShirleysRefract(RayIn.Direction, outward_normal, ni_over_nt, refracted))
		{
			reflect_prob = ShirleysSchlick(cosine, ior);
		}
		else
		{
			Scattered = ray(Rec.p, reflected);
			reflect_prob = 1.0f;
		}
		
		if(RandNum0ToLessThan1() < reflect_prob)
		{
			Scattered = ray(Rec.p, reflected);
		}
		else
		{
			Scattered = ray(Rec.p, refracted);
			
		}
		return true;
#else
		Attenuation = vec3(1.0, 1.0, 1.0);
		
		vec3 Reflected = Reflect(MakeUnitVector(RayIn.Direction), Rec.Normal); 
		vec3 Transmitted;
		float RefProb = Fresnel(RayIn.Direction, Rec.Normal, 1.0f, ior);
		
		if(RefProb < 1.0f)
		{
			Refract(RayIn.Direction, Rec.Normal, 1.0f, ior, Transmitted);
			//RefProb = Schlick(RayIn.Direction, Rec.Normal, 1.0f, ior);
		}
		
		if(RandNum0ToLessThan1() < RefProb)
		{
			Scattered = ray(Rec.p, Reflected, RayIn.Time);
		}
		else
		{
			Scattered = ray(Rec.p, Transmitted, RayIn.Time);			
		}
		return true;
#endif
        
	}
};

class diffuse_light : public material
{
	public:
	shared_ptr<texture> Emitted;
	diffuse_light(shared_ptr<texture> Emitted_) : Emitted(Emitted_){};
	virtual bool Scatter(const ray& RayIn, const hit_record& Rec, vec3& attenuation, ray& Scattered) const 
	{ 
		return false; 
	}
	
	virtual vec3 Emit(float u, float v, vec3 p)
	{
		return Emitted->Value(u,v,p);
	}
	
	~diffuse_light() {};
};



#endif


