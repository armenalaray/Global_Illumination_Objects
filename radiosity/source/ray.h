/* date = March 16th 2021 6:44 pm */

#ifndef RAY_H
#define RAY_H

#include "vec3.h"

class Ray
{
	public:
	Ray(const Vec3<float> & Origin_, const Vec3<float> & Direction_) : 
    Origin{Origin_}, Direction{Direction_}{}
	
    private:
	Vec3<float> Origin;
	Vec3<float> Direction;
};


#endif //RAY_H
