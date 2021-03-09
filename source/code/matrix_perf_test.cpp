//////////////////////////////////////////////////////
//
//Author: Alejandro Armenta
//Date: 2019
//File: matrix_perf_test.cpp
//Company:
//
//(2018-2019) All rights reserved.
//
//////////////////////////////////////////////////////
#include "pathtracer.h"

#define MAX_ITER 10e5

#if 1
int main()
{
	matrix44d Inverted;
	//matrix44d Original(0.707107, 0, -0.707107, 0, -0.331295, 0.883452, -0.331295, 0, 0.624695, 0.468521, 0.624695, 0, 4.000574, 3.00043, 4.000574, 1); 
    
#if 0
    matrix44d Original(
                       2,0,0,0,
                       0,0,1,0,
                       0,1,0,0,
                       0,0,0,1);
#endif
    
#if 0	
	matrix44d Original(
                       5,-5,5,6,
                       2,1,1,2,
                       -1,-1,0,1,
                       5,1,2,1);
#endif
    
    
#if 0
	matrix44d Original(
                       1, 1, 1, 1,
                       1, 1,-1,-1,
                       1,-1, 1,-1,
                       1,-1,-1, 1);
#endif
	
    matrix44d Original(
                       1, 1, 0,  0,
                       3, 6, 1, -1,
                       3, 9, 6,  0,
                       1, 0, 1,  1
                       );
    
	std::cerr << Original << std::endl; 
	const double * b = Original.operator[](0);
	
	const double a = Original[0][0];
	Original[0][1] = 1.0;
	//double * a = Original[0][0];
	//a = 2;
	//std::cerr << Original[0][0] << std::endl;
	
	
	Original.Invert(Inverted); 
    std::cerr << Inverted << std::endl; 
	
#if 0
	
#ifdef ROWMAJOR
	fprintf(stderr, "ROWMAJOR\n");
#else
	fprintf(stderr, "ROWMINOR\n");
#endif
	
	clock_t start = clock();
	float * tmp = &M.m[0][0];
	for(int i = 0; i < 16; i++) *(tmp + i) = RandNum0ToLessThan1();
	
	for(int i = 0; i < MAX_ITER; ++i)
	{
		vec3 vt = M.MultV(v);
	}
	
	fprintf(stderr, "Clock time %f\n", (clock() - start) / float(CLOCKS_PER_SEC));
#endif
	return 0;
}
#endif
