//////////////////////////////////////////////////////
//
//Author: Alejandro Armenta
//Date: 2019
//File: pathtracer_random.h
//Company:
//
//(2018-2019) All rights reserved.
//
//////////////////////////////////////////////////////


#ifndef PATHTRACER_RANDOM_H
#define PATHTRACER_RANDOM_H

void SeedInit()
{
	srand((unsigned)time(NULL));
}

float RandNum0ToLessThan1()
{
	// NOTE(Alex): I want a number from 0 <= rand < 1 
	float Result = (float)rand() / (float)((RAND_MAX) + 1);  
	return Result;	
}

vec3 RUVXY()
{
	vec3 Result = vec3(RandNum0ToLessThan1(), RandNum0ToLessThan1(), 0.0f);
	Result.MakeUnitVector();
	return Result;
}


vec3 RandomInUnitDisc()
{
	vec3 Result(1.0,0,0);
	
	while(Result.SquaredLength() >= 1.0)
		Result = ((2.0f * vec3(RandNum0ToLessThan1(), RandNum0ToLessThan1(), 0.0f)) - vec3(1.0f,1.0f,0.0f));
	
	return Result;
}

vec3 RandomInUnitDiscXY()
{
	vec3 Result(1.0,0,0);
	
	while(Result.SquaredLength() >= 1.0)
		Result = ((2.0f * vec3(RandNum0ToLessThan1(), RandNum0ToLessThan1(), 0.0f)) - vec3(1.0f,1.0f,0.0f));
	Result.MakeUnitVector();
	return Result;
}

vec3 RandomInUnitSphere()
{
	vec3 Result(1.0,0,0);
	
	while(Result.SquaredLength() >= 1.0)
		Result = ((2.0f * vec3(RandNum0ToLessThan1(), RandNum0ToLessThan1(), RandNum0ToLessThan1())) - vec3(1.0f,1.0f,1.0f));
	
	return Result;
}


#endif


