//////////////////////////////////////////////////////
//
//Author: Alejandro Armenta
//Date: 2019
//File: pathtracer.h
//Company:
//
//(2018-2019) All rights reserved.
//
//////////////////////////////////////////////////////


#ifndef PATHTRACER_LT_H
#define PATHTRACER_LT_H

/*
// NOTE(Alex): so we are gonna try to capture all the importance paths and store the data into a file,
then I´m gonna use it to compute the rendering as a postprocess!
*/


vec3 Color(ray R, shared_ptr<hitable> World, int Depth)
{
	hit_record Rec;
	if(World->Hit(R, 0.001f, FLT_MAX, Rec))
	{
		vec3 Attenuation;
		ray Scattered;
		vec3 Emitted = Rec.Material->Emit(Rec.u, Rec.v, Rec.p);
		if(Depth < 50 && Rec.Material->Scatter(R, Rec, Attenuation, Scattered))
		{
			// NOTE(Alex): This is the light transport equation!!!
		    return Emitted + Attenuation * Color(Scattered, World, Depth + 1);
		}
		else
		{
			return Emitted;
		}
	}
	else
	{
		return vec3(0,0,0);
	}	
}

vec3 PotentialTransport(ray R, shared_ptr<hitable> World, int Depth)
{
	hit_record Rec;
	if(World->Hit(R, 0.001f, FLT_MAX, Rec))
	{
		//vec3 RandomP = rec.p + rec.Normal + RandomInUnitSphere();
		vec3 Attenuation;
		ray Scattered;
		//vec3 Emitted = Rec.Material->Emit(Rec.u, Rec.v, Rec.p);
		if(Rec.Material->Scatter(R, Rec, Attenuation, Scattered) && Depth < 50)
		{
			// NOTE(Alex): This is the light transport equation!!!
		    return Attenuation * PotentialTransport(Scattered, World, Depth + 1);
		}
		else
		{
			return Attenuation;
		}
	}
	else
	{
		return vec3(0,0,0);
	}	
}




#endif