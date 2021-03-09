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

/*  
NOTE

To declare the object pointed to by the pointer as const or volatile, use a declaration of the form
const char * constobjectptr;
volatile char * volatileobjectptr;

To declare the value of the pointer - that is, the actual address stored in the pointer as const or volatile, 
use a declaration of the form:

char * const constaddressofcharptr;
char * volatile volatileaddressofcharptr;

So a pathtracer computes the color seen from the eye on each pixel.

so i want a raytracer that throws rays to each pixel, and brings back a color back.

SSS implementation. 

So i need to make a raytracer from the surface of the subsurface scattering material.
A raytracer within a raytracer.

So that all light propagation can be accumulated in the spot of the surface and i can simulate it!!

*/


#ifndef PATHTRACER_H
#define PATHTRACER_H

//#include "Windows.h"
#include <vector>
#include <iostream>
#include <sstream>
#include <iomanip>

#include <fstream>
#include <random>
#include <functional>

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include <time.h>

#define _USE_MATH_DEFINES
#include "math.h"
#include "float.h"


#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

using namespace std;

#define TEST_100_ELEM_PER_FACE

//#define STB_IMAGE_WRITE_IMPLEMENTATION
//#include "stb_image_write.h"

#ifdef _DEBUG
#define Assert(Statement) if((Statement) == false) { *(int *)0 = 0; } 
#define InvalidDefaultCase default:Assert(false);break;
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
// Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
// allocations to be of _CLIENT_BLOCK type
#else
#define Assert(Statement)
#define InvalidDefaultCase 
#define DBG_NEW new
#endif

#define ArrayCount(array) (sizeof((array)) / sizeof((array)[0]))
#define NV255(c) ((float)(c) / 255.0f)

#define KILOBYTES(i) (1024 * i)
#define MEGABYTES(i) (1024 * KILOBYTES(i))
#define GIGABYTES(i) (1024 * MEGABYTES(i))


class vec3;
class material;
class lambertian;

#include "pathtracer_math.h"
#include "pathtracer_random.h"
#include "pathtracer_noise.h"
#include "pathtracer_texture.h"
#include "pathtracer_hitable.h"
#include "pathtracer_camera.h"
#include "pathtracer_material.cpp"
#include "pathtracer_material.h"
#include "pathtracer_volume.h"
#include "pathtracer_lt.h"

#include "radiosity_memory.h"
#include "radiosity_math.h"
#include "radiosity_hitable.h"
#include "radiosity_vertex.h"
#include "radiosity_mesh.h"

#endif


