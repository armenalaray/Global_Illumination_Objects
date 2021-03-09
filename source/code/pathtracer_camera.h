//////////////////////////////////////////////////////
//
//Author: Alejandro Armenta
//Date: 2019
//File: pathtracer_camera.h
//Company:
//
//(2018-2019) All rights reserved.
//
//////////////////////////////////////////////////////


#ifndef PATHTRACER_CAMERA_H
#define PATHTRACER_CAMERA_H

class camera
{
	public:
	float LensRadius;
	vec3 LowerLeftCorner;
	vec3 Origin;
	vec3 Horizontal;
	vec3 Vertical;
	vec3 u, v, w;
	float Time0,Time1;
	
	camera(const vec3 LookFrom, const vec3 LookAt, const vec3 VUP, float VFOVDeg, float AspectRatio, float Aperture, float FocusDistance, float Time0_, float Time1_) 
	{
		Time0 = Time0_;
		Time1 = Time1_;
		LensRadius = Aperture / 2.0f;
		float fovRadians = VFOVDeg * (float)M_PI / 180.0f;
		
		float HalfHeight = tan(fovRadians / 2.0f);// NOTE(Alex): We are assuming that z.Length() = 1.
		float HalfWidth = AspectRatio * HalfHeight;
		
		// NOTE(Alex): We make camera ortonormal basis
		w = MakeUnitVector(LookFrom - LookAt);
		u = MakeUnitVector(Cross(VUP, w));
		v = Cross(w,u);
		
		Origin = LookFrom;
		// NOTE(Alex): We multiply w by focus distance and to keep the same angle we need to
		// multiply u and v by focus distance
		LowerLeftCorner = (Origin - (HalfWidth * FocusDistance * u) - (HalfHeight * FocusDistance * v) - w * FocusDistance);
		Horizontal = 2.0f * HalfWidth * FocusDistance * u;
		Vertical = 2.0f * HalfHeight * FocusDistance * v;
	}

	ray GetRay(float s, float t) 
	{
		Assert(LensRadius >= 0);
		// NOTE(Alex): we will randomize ray origin in a disc.
		vec3 r = LensRadius * RandomInUnitDisc();
		vec3 Offset = u * r.X() + v * r.Y();
		vec3 p = Origin + Offset;
		
		float Time = Time0 + RandNum0ToLessThan1() * (Time1 - Time0);
		
		ray Result = ray(p, LowerLeftCorner + s * Horizontal + t * Vertical - p, Time);
		return Result;
	}
};

#endif


