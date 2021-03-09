//////////////////////////////////////////////////////
//
//Author: Alejandro Armenta
//Date: 2019
//File: pathtracer_volume.h
//Company:
//
//(2018-2019) All rights reserved.
//
//////////////////////////////////////////////////////


#ifndef PATHTRACER_VOLUME_H
#define PATHTRACER_VOLUME_H


class constant_medium : public hitable
{
	public:
	float Density;
	shared_ptr<hitable> Boundary;
	shared_ptr<material> PhaseFunc;
	constant_medium(float Density_, shared_ptr<hitable> Boundary_, shared_ptr<texture> a) : 
	Density(Density_), 
	Boundary(Boundary_)
	{
		// TODO(Alex): PhaseFunc
		//PhaseFunc(a) 
	}
	virtual bool Hit(const ray& r, float tMin, float tMax, hit_record& rec) const
	{
		hit_record rec1, rec2;
		if(Boundary->Hit(r, -FLT_MAX, FLT_MAX, rec1))
		{
			if(Boundary->Hit(r, rec1.t + 0.0001f, FLT_MAX, rec2))
			{
				if(rec1.t < tMin) rec1.t = tMin;
				if(rec2.t > tMax) rec2.t = tMax;
				if(rec1.t >= rec2.t) return false;
				if(rec1.t < 0) rec1.t = 0;
				
				float DIB = (rec2.t - rec1.t)*r.Direction.Length();
				float HD = -(1.0f/Density) * log(RandNum0ToLessThan1());
				
				if(HD < DIB)
				{
					rec.t = rec1.t + HD / r.Direction.Length();
					rec.p = r.AtPoint(rec.t);
					rec.Normal = vec3(1,0,0); //arbitrary
					rec.Material = PhaseFunc;
					return true;
				}
			}
		}
		return false;
	}
	
	virtual bool BBox(float Time0, float Time1, aabb & OutBBox) const
	{
		return Boundary->BBox(Time0, Time1, OutBBox);
	}
	
	virtual bool Center(vec3 & OutCenter) const 
	{ 
		return false; 
	}
};

#endif


