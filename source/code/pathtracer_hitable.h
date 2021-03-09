//////////////////////////////////////////////////////
//
//Author: Alejandro Armenta
//Date: 2019
//File: pathtracer_hitable.h
//Company:
//
//(2018-2019) All rights reserved.
//
//////////////////////////////////////////////////////


#ifndef PATHTRACER_HITABLE_H
#define PATHTRACER_HITABLE_H


struct hit_record
{
	float t;
	float u;
	float v;
	vec3 p;
	vec3 Normal;
	shared_ptr<material> Material;
};

class hitable
{
	public:
	virtual bool Hit(const ray& r, float tMin, float tMax, hit_record& rec) const = 0;
	virtual bool BBox(float Time0, float Time1, aabb & OutBBox) const = 0;
	virtual bool GetCenter(vec3 & OutCenter) const { return false; }
	virtual ~hitable() = 0;
};

hitable::~hitable(){}


class triangle : public hitable
{
	public:
	vec3 v0, v1, v2, n;
	shared_ptr<material> Material;
	float Area2;
	
	triangle() {}
	
	triangle(vec3 v0_, vec3 v1_, vec3 v2_, shared_ptr<material> Material_) : v0(v0_), v1(v1_), v2(v2_), Material(Material_) 
	{		
		vec3 A = v1 - v0;
		vec3 B = v2 - v0;
		n = Cross(A, B);
		Area2 = n.SquaredLength();
	}
	
	virtual bool Hit(const ray& r, float tMin, float tMax, hit_record& rec) const
	{
		
#define MOLLER_TRUMBORE 1
#ifdef MOLLER_TRUMBORE		
		vec3 E1 = v1 - v0;
		vec3 E2 = v2 - v0;
		vec3 D = r.Direction;
		vec3 P = Cross(D, E2);
		float InvDet = Dot(P, E1);
		if(abs(InvDet) < FLT_EPSILON) return false;
		InvDet = 1.0f / InvDet;
		vec3 T = r.Origin - v0;
		vec3 Q = Cross(T, E1);
		float u = Dot(P, T)  * InvDet;
		if(u < 0 || u > 1.0f) return false;
		float v = Dot(Q, D) * InvDet;
		if(v < 0 || u + v > 1.0f) return false;
		float t = Dot(Q, E2) * InvDet; 
		if(t < tMin || t > tMax) return false; 
		rec.t = t;
		rec.u = u;
		rec.v = v;
		rec.p = r.AtPoint(rec.t);
		rec.Normal = MakeUnitVector(n);
		rec.Material = Material;
		return true;
#else
		float InvRP = Dot(r.Direction, n);
		//if(InvRP < -FLT_EPSILON) // NOTE(Alex): one sided triangles
		if(abs(InvRP) > FLT_EPSILON)
		{
			InvRP = 1.0f / InvRP;
			float t = Dot(v0 - r.Origin, n) * InvRP;
			
			if(t > tMin && t < tMax)
			{
				vec3 e0 = v1 - v0;
				vec3 e1 = v2 - v1;
				vec3 e2 = v0 - v2;
				
				vec3 p = r.AtPoint(t);
				vec3 p0 = p - v0;
				vec3 p1 = p - v1;
				vec3 p2 = p - v2;
				
				vec3 n0 = Cross(e0, p0);
				vec3 n1 = Cross(e1, p1);
				vec3 n2 = Cross(e2, p2);
				
				float a = Dot(n,n0); 
				float b = Dot(n,n1); 
				float c = Dot(n,n2); 
				
				if(a < 0.0f || b < 0.0f || c < 0.0f) return false;
				
				rec.t = t;
				rec.u = c / Area2;
				rec.v = a / Area2;
				rec.p = r.AtPoint(t);
				rec.Normal = MakeUnitVector(n);
				rec.Material = Material.get();
				return true;
			}
		}
		
		return false;
#endif
	}
	
	virtual bool BBox(float Time0, float Time1, aabb & OutBBox) const
	{
		
		OutBBox = aabb(
                       vec3(
                            std::min(std::min(v0.x, v1.x), v2.x) - 0.01f, 
                            std::min(std::min(v0.y, v1.y), v2.y) - 0.01f, 
                            std::min(std::min(v0.z, v1.z), v2.z) - 0.01f),
                       vec3(
                            std::max(std::max(v0.x, v1.x), v2.x) + 0.01f, 
                            std::max(std::max(v0.y, v1.y), v2.y) + 0.01f, 
                            std::max(std::max(v0.z, v1.z), v2.z) + 0.01f));
		return true;		
	}
	
	virtual bool GetCenter(vec3 & OutCenter) const { return false; }
};


class triangle_mesh : public hitable
{
	public:
	shared_ptr<material> Material;
	vector<shared_ptr<hitable>> Mesh;
	
	triangle_mesh() {}
	triangle_mesh(std::string File, shared_ptr<material> Material_) : Material(Material_) 
	{
#if 0		
        vec3 Vertices[6] = 
		{ 
            vec3 (1.0f, 1.0f, 4.0f), 
            vec3 (3.0f, 1.0f, 4.0f), 
            vec3 (3.0f, 1.0f, 2.0f), 
            vec3 (1.0f, 1.0f, 2.0f), 
            vec3 (1.0f, 3.0f, 2.0f), 
            vec3 (1.0f, 3.0f, 4.0f)
		};
        uint32_t NumFaces = 2; // number of faces 
        uint32_t FaceIndex[2] = {4, 4}; // face index array 
        uint32_t VertexIndices[8] = {0, 1, 2, 3, 0, 3, 4, 5}; // vertex index array 
        
        int Offset = 0;
        for(uint32_t i = 0; i < NumFaces; i++)
        {
            for(uint32_t j = 0; j < (FaceIndex[i] - 2); ++j)
            {
                vec3 v0 = Vertices[VertexIndices[Offset]]; 
                vec3 v1 = Vertices[VertexIndices[Offset + j + 1]]; 
                vec3 v2 = Vertices[VertexIndices[Offset + j + 2]];
                Mesh.push_back(make_shared<triangle>(v0,v1,v2, Material));
            }
            Offset += FaceIndex[i];
        }
#endif
		std::ifstream ifs;
		
		try
		{
			ifs.open(File);
			if(ifs.fail()) throw;
			std::stringstream ss;
			ss << ifs.rdbuf();
			uint32_t NumFaces;
			ss >> NumFaces;
			std::unique_ptr<uint32_t []> FaceIndex(new uint32_t[NumFaces]);
			
			// NOTE(Alex): reading face index array
			uint32_t VertIndexArraySize = 0;
			for(uint32_t i = 0; i < NumFaces; ++i)
			{
				ss >> FaceIndex[i];
				VertIndexArraySize += FaceIndex[i];
			}
			
			// NOTE(Alex): reading vertexIndexArray
			std::unique_ptr <uint32_t []> VertexIndices(new uint32_t[VertIndexArraySize]);
			
			uint32_t VertArraySize = 0;
			for(uint32_t i = 0; i < VertIndexArraySize; i++)
			{
				ss >> VertexIndices[i];
				if(VertexIndices[i] > VertArraySize) VertArraySize = VertexIndices[i];
			}
			
			VertArraySize += 1;
			
			// NOTE(Alex): reading vertices
			std::unique_ptr<vec3[]> Vertices(new vec3[VertArraySize]);
			for(uint32_t i = 0; i < VertArraySize; ++i)
			{
				ss >> Vertices[i].x >> Vertices[i].y >> Vertices[i].z;
			}
			
			
			// NOTE(Alex): reading normals
			std::unique_ptr<vec3[]> Normals(new vec3[VertIndexArraySize]);
			for(uint32_t i = 0; i < VertIndexArraySize; ++i)
			{
				ss >> Normals[i].x >> Normals[i].y >> Normals[i].z;
			}
			
			
			// NOTE(Alex): reading st coordinates
			std::unique_ptr<vec2f[]> UVs(new vec2f[VertIndexArraySize]);
			for(uint32_t i = 0; i < VertIndexArraySize; ++i)
			{
				ss >> UVs[i].x >> UVs[i].y;
			}
			
			int Offset = 0;
			for(uint32_t i = 0; i < NumFaces; i++)
			{
				for(uint32_t j = 0; j < (FaceIndex[i] - 2); ++j)
				{
					vec3 v0 = Vertices[VertexIndices[Offset]]; 
					vec3 v1 = Vertices[VertexIndices[Offset + j + 1]]; 
					vec3 v2 = Vertices[VertexIndices[Offset + j + 2]];
					Mesh.push_back(make_shared<triangle>(v0,v1,v2, Material));
				}
				Offset += FaceIndex[i];
			}
		}
		catch(...)
		{
			ifs.close();
		}
		
		ifs.close();
        
	}
	
	virtual bool Hit(const ray& r_, float tMin_, float tMax_, hit_record& rec_) const
	{
		bool Result = false;
		float tMax = tMax_;
		hit_record hitTemp = {};
		for(int i = 0; i < Mesh.size(); ++i)
		{
			if(Mesh[i]->Hit(r_, tMin_, tMax, hitTemp))
			{
				tMax = hitTemp.t;
				rec_ = hitTemp;
				Result = true;
			}
		}
		return Result;
	}
	
	virtual bool BBox(float Time0, float Time1, aabb & OutBBox) const
	{
		return false;
	}
	
	virtual bool GetCenter(vec3 & OutCenter) const 
	{ 
		return false; 
	}	
	
};


class flip_normals : public hitable
{
	public:
	shared_ptr<hitable> Hitable;
	flip_normals();
	flip_normals(shared_ptr<hitable> Other) : Hitable(Other) {} 
	virtual bool Hit(const ray& r, float tMin, float tMax, hit_record& rec) const
	{
		bool Result = Hitable->Hit(r, tMin, tMax, rec); 
		if(Result)
		{
			rec.Normal = -rec.Normal;
		}
		return Result;
	}
	
	virtual bool BBox(float Time0, float Time1, aabb & OutBBox) const
	{
		return Hitable->BBox(Time0, Time1, OutBBox);
	}	
};

class xy_rect : public hitable
{
	public:
	
	float x0, x1, y0, y1, k;
	shared_ptr<material> Material;
	
	xy_rect();
	xy_rect(float x0_, float x1_, float y0_, float y1_, float k_, shared_ptr<material> Material_) : 
	x0(x0_), 
	x1(x1_), 
	y0(y0_), 
	y1(y1_), 
	k(k_), 
	Material(Material_) 
	{}
	
	virtual bool Hit(const ray& r, float tMin, float tMax, hit_record& rec) const;
	virtual bool BBox(float Time0, float Time1, aabb & OutBBox) const
	{
		OutBBox = aabb(vec3(x0,y0, k - 0.0001f), vec3(x1,y1, k + 0.0001f));
		return true;
	}	
};

bool xy_rect::Hit(const ray& r, float tMin, float tMax, hit_record& rec) const
{
	float t = (k - r.Origin.z) / r.Direction.z;
	
	if(t < tMin || t > tMax) return false;
	
	float x = r.Origin.x + t * r.Direction.x;
	float y = r.Origin.y + t * r.Direction.y;
	
	if(x < x0 || x > x1 || y < y0 || y > y1) return false;
	
	rec.t = t;
	
    rec.u = ((x - x0) / (x1 - x0));
	rec.v = ((y - y0) / (y1 - y0));
	
    rec.p = r.AtPoint(t);
	rec.Normal = vec3(0,0,1);
	rec.Material = Material;
	
	return true;
}


class xz_rect : public hitable
{
	public:
	
	float x0, x1, z0, z1, k;
	shared_ptr<material> Material;
	
	xz_rect();	
	xz_rect(float x0_, float x1_, float z0_, float z1_, float k_, shared_ptr<material> Material_) : 
	x0(x0_), 
	x1(x1_), 
	z0(z0_), 
	z1(z1_), 
	k(k_), 
	Material(Material_) 
	{}
	
	virtual bool Hit(const ray& r, float tMin, float tMax, hit_record& rec) const;
	virtual bool BBox(float Time0, float Time1, aabb & OutBBox) const
	{
		OutBBox = aabb(vec3(x0, k - 0.0001f, z0), vec3(x1, k + 0.0001f, z1));
		return true;
	}	
};

bool xz_rect::Hit(const ray& r, float tMin, float tMax, hit_record& rec) const
{
	float t = (k - r.Origin.y) / r.Direction.y;
	
	if(t < tMin || t > tMax) return false;
	
	float x = r.Origin.x + t * r.Direction.x;
	float z = r.Origin.z + t * r.Direction.z;
	
	if(x < x0 || x > x1 || z < z0 || z > z1) return false;
	
	rec.t = t;
	rec.u = ((z - z0) / (z1 - z0));
	rec.v = ((x - x0) / (x1 - x0));
	rec.p = r.AtPoint(t);
	rec.Normal = vec3(0,1,0);
	//rec.Material = Material.get();
	rec.Material = Material;
	
	return true;
}

class yz_rect : public hitable
{
	public:
	
	float y0, y1, z0, z1, k;
	shared_ptr<material> Material;
	
	yz_rect();
	yz_rect(float y0_, float y1_, float z0_, float z1_, float k_, shared_ptr<material> Material_) : 
	y0(y0_), 
	y1(y1_), 
	z0(z0_), 
	z1(z1_), 
	k(k_), 
	Material(Material_) 
	{}
	
	virtual bool Hit(const ray& r, float tMin, float tMax, hit_record& rec) const;
	virtual bool BBox(float Time0, float Time1, aabb & OutBBox) const
	{
		OutBBox = aabb(vec3(k - 0.0001f, y0, z0), vec3(k + 0.0001f, y1, z1));
		return true;
	}	
};

bool yz_rect::Hit(const ray& r, float tMin, float tMax, hit_record& rec) const
{
	float t = (k - r.Origin.x) / r.Direction.x;
	
	if(t < tMin || t > tMax) return false;
	
	float y = r.Origin.y + t * r.Direction.y;
	float z = r.Origin.z + t * r.Direction.z;
	
	if(y < y0 || y > y1 || z < z0 || z > z1) return false;
	
	rec.t = t;
	rec.u = ((z - z0) / (z1 - z0));
	rec.v = ((y - y0) / (y1 - y0));
	rec.p = r.AtPoint(t);
	rec.Normal = vec3(1,0,0);
	rec.Material = Material;	
	return true;
}



class yz_rect_grid : public hitable
{
	public:
	
    int WidthCount, HeightCount;
	float y0, y1, z0, z1, k;
	shared_ptr<material> Material;
	
	yz_rect_grid();
	yz_rect_grid(int WidthCount_, int HeightCount_, float y0_, float y1_, float z0_, float z1_, float k_, shared_ptr<material> Material_) : 
	WidthCount(WidthCount_), 
	HeightCount(HeightCount_), 
	y0(y0_), 
	y1(y1_), 
	z0(z0_), 
	z1(z1_), 
	k(k_), 
	Material(Material_) 
	{}
	
	virtual bool Hit(const ray& r, float tMin, float tMax, hit_record& rec) const;
	virtual bool BBox(float Time0, float Time1, aabb & OutBBox) const
	{
		OutBBox = aabb(vec3(k - 0.0001f, y0, z0), vec3(k + 0.0001f, y1, z1));
		return true;
	}	
};

bool yz_rect_grid::Hit(const ray& r, float tMin, float tMax, hit_record& rec) const
{
	float t = (k - r.Origin.x) / r.Direction.x;
	
	if(t < tMin || t > tMax) return false;
	
	float y = r.Origin.y + t * r.Direction.y;
	float z = r.Origin.z + t * r.Direction.z;
	
	if(y < y0 || y > y1 || z < z0 || z > z1) return false;
	
	rec.t = t;
	rec.u = 1.0f - ((y - y0) / (y1 - y0));
	rec.v = 1.0f - ((z - z0) / (z1 - z0));
	rec.p = r.AtPoint(t);
	rec.Normal = vec3(1,0,0);
	rec.Material = Material;	
	return true;
}


class hitable_list : public hitable
{
	public:
	vector<shared_ptr<hitable>> List;
	hitable_list(vector<shared_ptr<hitable>> List_)
	{
		List = List_;
	}
	
	virtual bool BBox(float Time0, float Time1, aabb & OutBBox) const;
	virtual bool Hit(const ray& r, float tMin, float tMax, hit_record& rec) const;
};

bool hitable_list::BBox(float Time0, float Time1, aabb & OutBBox) const
{
	bool Result = false;
	aabb TBBox = OverlappedBBox();
	OutBBox = OverlappedBBox();	
	int i = 0;
	for(; i < List.size(); i++)
	{
		if(List[i]->BBox(Time0, Time1, TBBox))
		{
			OutBBox = UnionAABB(TBBox, OutBBox); 
		}
	}
	
	if(i == List.size())
		Result = true;
	return Result;
}

bool hitable_list::Hit(const ray& r_, float tMin_, float tMax_, hit_record& rec_) const
{
	bool Result = false;
	float tMax = tMax_;
	hit_record hitTemp = {};
	for(int i = 0; i < List.size(); ++i)
	{
		if(List[i]->Hit(r_, tMin_, tMax, hitTemp))
		{
			tMax = hitTemp.t;
			rec_ = hitTemp;
			Result = true;
		}
	}
	return Result;
}

class box : public hitable
{
	public:
	vec3 Min, Max;	
	shared_ptr<hitable> HitableList;
	
	box();
	box(vec3 Min_, vec3 Max_, shared_ptr<material> Material);
	virtual bool Hit(const ray& r, float tMin, float tMax, hit_record& rec) const;
	virtual bool BBox(float Time0, float Time1, aabb & OutBBox) const 
	{ 
		OutBBox = aabb(Min, Max); 
		return true;
	}
	
	virtual bool GetCenter(vec3 & OutCenter) const 
	{ 
		vec3 v = Max - Min;
		OutCenter = Min + 0.5f * v;
		return true; 
	}
	~box() {};
};

box::box(vec3 Min_, vec3 Max_, shared_ptr<material> Material) : Min(Min_), Max(Max_) 
{
	vector<shared_ptr<hitable>> List;
	
	shared_ptr<hitable> yz0 = make_shared<yz_rect>(Min.y, Max.y, Min.z, Max.z, Min.x, Material);
	shared_ptr<hitable> xy0 = make_shared<xy_rect>(Min.x, Max.x, Min.y, Max.y, Min.z, Material);
	shared_ptr<hitable> xz0 = make_shared<xz_rect>(Min.x, Max.x, Min.z, Max.z, Min.y, Material);
	
	List.push_back(make_shared<yz_rect>(Min.y, Max.y, Min.z, Max.z, Max.x, Material));
	List.push_back(make_shared<flip_normals>(yz0));
	List.push_back(make_shared<xy_rect>(Min.x, Max.x, Min.y, Max.y, Max.z, Material));
	List.push_back(make_shared<flip_normals>(xy0));
	List.push_back(make_shared<xz_rect>(Min.x, Max.x, Min.z, Max.z, Max.y, Material));
	List.push_back(make_shared<flip_normals>(xz0));
    
	HitableList = make_shared<hitable_list>(List);
}

bool box::Hit(const ray& r, float tMin, float tMax, hit_record& rec) const
{
	bool Result = HitableList->Hit(r, tMin, tMax, rec);
	return Result;
}

class translate : public hitable
{
	public:
	vec3 Offset;
	shared_ptr<hitable> Ptr;
	translate();
	translate(vec3 Offset_, shared_ptr<hitable> Ptr_) : Offset(Offset_), Ptr(Ptr_) {}
	
	// NOTE(Alex): Hit translates the ray minus offset. so we can simulate translation.
	
	virtual bool Hit(const ray& r, float tMin, float tMax, hit_record& rec) const
	{
		ray MovedRay = ray(r.Origin - Offset, r.Direction, r.Time);
		if(Ptr->Hit(MovedRay, tMin, tMax, rec))
		{
			rec.p += Offset;
			return true;
		}
		return false;
	}
	
	// NOTE(Alex): We calculate the bbox before any hit call is made,  cause bbox are created on the constructor of bvh node
	// therefore we need to translate bbox for correct aabb testing
	
	virtual bool BBox(float Time0, float Time1, aabb & OutBBox) const
	{
		if(Ptr->BBox(Time0, Time1, OutBBox))
		{
			OutBBox = aabb(OutBBox.Min + Offset, OutBBox.Max + Offset);
			return true;
		}
		return false;
	}
};


class rotate_z : public hitable
{
	public:
	
	shared_ptr<hitable> Ptr;
	float Beta;
	vec3 PtrCenter;
    
	aabb RBBox;
	bool HasBBox;
	
	rotate_z();
	rotate_z(float Angle_, shared_ptr<hitable> Ptr_) : Ptr(Ptr_) 
	{		
		Ptr->GetCenter(PtrCenter);
		Beta = Angle_ * (float)M_PI / 180.0f;
	}
	
	virtual bool GetCenter(vec3 & OutCenter) const 
	{ 
		OutCenter = PtrCenter; 
		return true;
	}
	
	// NOTE(Alex): Hit translates the ray minus offset. so we can simulate translation.
	// TODO(Alex): CHECK THIS!! Lengths are not correct!
	virtual bool Hit(const ray& r, float tMin, float tMax, hit_record& rec) const
	{			
		vec3 roo = r.Origin - PtrCenter;
		vec3 nrov = RotateZCounterClockWise(-Beta, roo);		
		vec3 nro = nrov + PtrCenter;
		
		vec3 rdo = r.Direction;		
		vec3 nrdv = RotateZCounterClockWise(-Beta, rdo);		
		vec3 nrd = nrdv;
		
		ray MovedRay = ray(nro, nrd, r.Time);
		if(Ptr->Hit(MovedRay, tMin, tMax, rec))
		{
			vec3 n = rec.Normal;
			vec3 nv = RotateZCounterClockWise(Beta, n);		
			rec.Normal = nv;
			
			vec3 a = rec.p - PtrCenter;
			vec3 av = RotateZCounterClockWise(Beta, a);					
			rec.p = av + PtrCenter;
			return true;
		}
		return false;
	}
	
	
	// NOTE(Alex): We calculate the bbox before any hit call is made,  cause bbox are created on the constructor of bvh node
	// therefore we need to translate bbox for correct aabb testing
	
	virtual bool BBox(float Time0, float Time1, aabb & OutBBox) const
	{
		if(Ptr->BBox(Time0, Time1, OutBBox))
		{
			vec3 Points[8] = {
				vec3(OutBBox.Min.x, OutBBox.Min.y, OutBBox.Min.z),
				vec3(OutBBox.Max.x, OutBBox.Min.y, OutBBox.Min.z),
				vec3(OutBBox.Max.x, OutBBox.Min.y, OutBBox.Max.z),
				vec3(OutBBox.Min.x, OutBBox.Min.y, OutBBox.Max.z),
				vec3(OutBBox.Min.x, OutBBox.Max.y, OutBBox.Min.z),
				vec3(OutBBox.Max.x, OutBBox.Max.y, OutBBox.Min.z),
				vec3(OutBBox.Max.x, OutBBox.Max.y, OutBBox.Max.z),
				vec3(OutBBox.Min.x, OutBBox.Max.y, OutBBox.Max.z),
			};
			
			for(int i = 0; i < 8; i++)
			{
				Points[i] -= PtrCenter;
				Points[i] = RotateZCounterClockWise(Beta, Points[i]);
				Points[i] += PtrCenter;
			}
			
			OutBBox = aabb(Points, ArrayCount(Points));
			return true;
		}
		return false;
	}	
};


class rotate_y : public hitable
{
	public:
	
	shared_ptr<hitable> Ptr;
	float Beta;
	vec3 PtrCenter;
    
	aabb RBBox;
	bool HasBBox;
	
	rotate_y();
	rotate_y(float Angle_, shared_ptr<hitable> Ptr_) : Ptr(Ptr_) 
	{		
		Ptr->GetCenter(PtrCenter);
		Beta = Angle_ * (float)M_PI / 180.0f;
	}
    
	virtual bool GetCenter(vec3 & OutCenter) const 
	{ 
		OutCenter = PtrCenter; 
		return true;
	}
	
	// NOTE(Alex): Hit translates the ray minus offset. so we can simulate translation.
	virtual bool Hit(const ray& r, float tMin, float tMax, hit_record& rec) const
	{			
		vec3 roo = r.Origin - PtrCenter;
		vec3 nrov = RotateYCounterClockWise(-Beta, roo);		
		vec3 nro = nrov + PtrCenter;
		
		vec3 rdo = r.Direction;		
		vec3 nrdv = RotateYCounterClockWise(-Beta, rdo);		
		vec3 nrd = nrdv;
		
		ray MovedRay = ray(nro, nrd, r.Time);
		if(Ptr->Hit(MovedRay, tMin, tMax, rec))
		{
			vec3 n = rec.Normal;
			vec3 nv = RotateYCounterClockWise(Beta, n);		
			rec.Normal = nv;
			
			vec3 a = rec.p - PtrCenter;
			vec3 av = RotateYCounterClockWise(Beta, a);					
			rec.p = av + PtrCenter;
			return true;
		}
		return false;
	}
	
	
	// NOTE(Alex): We calculate the bbox before any hit call is made,  cause bbox are created on the constructor of bvh node
	// therefore we need to translate bbox for correct aabb testing
	
	virtual bool BBox(float Time0, float Time1, aabb & OutBBox) const
	{
		if(Ptr->BBox(Time0, Time1, OutBBox))
		{
			vec3 Points[8] = {
				vec3(OutBBox.Min.x, OutBBox.Min.y, OutBBox.Min.z),
				vec3(OutBBox.Max.x, OutBBox.Min.y, OutBBox.Min.z),
				vec3(OutBBox.Max.x, OutBBox.Min.y, OutBBox.Max.z),
				vec3(OutBBox.Min.x, OutBBox.Min.y, OutBBox.Max.z),
				vec3(OutBBox.Min.x, OutBBox.Max.y, OutBBox.Min.z),
				vec3(OutBBox.Max.x, OutBBox.Max.y, OutBBox.Min.z),
				vec3(OutBBox.Max.x, OutBBox.Max.y, OutBBox.Max.z),
				vec3(OutBBox.Min.x, OutBBox.Max.y, OutBBox.Max.z),
			};
			
			for(int i = 0; i < 8; i++)
			{
				Points[i] -= PtrCenter;
				Points[i] = RotateYCounterClockWise(Beta, Points[i]);
				Points[i] += PtrCenter;
			}
			
			OutBBox = aabb(Points, ArrayCount(Points));
			return true;
		}
		return false;
	}
	
};


class rotate_x : public hitable
{
	public:
	
	shared_ptr<hitable> Ptr;
	float Beta;
	vec3 PtrCenter;
	
	rotate_x();
	rotate_x(float Angle_, shared_ptr<hitable> Ptr_) : Ptr(Ptr_) 
	{		
		Ptr->GetCenter(PtrCenter);
		Beta = Angle_ * (float)M_PI / 180.0f;
	}
    
	virtual bool GetCenter(vec3 & OutCenter) const 
	{ 
		OutCenter = PtrCenter; 
		return true;
	}
    
	// NOTE(Alex): Hit translates the ray minus offset. so we can simulate translation.
	virtual bool Hit(const ray& r, float tMin, float tMax, hit_record& rec) const
	{			
		vec3 roo = r.Origin - PtrCenter;
		vec3 nrov = RotateXCounterClockWise(-Beta, roo);		
		vec3 nro = nrov + PtrCenter;
		
		vec3 rdo = r.Direction;		
		vec3 nrdv = RotateXCounterClockWise(-Beta, rdo);		
		vec3 nrd = nrdv;
		
		ray MovedRay = ray(nro, nrd, r.Time);
		if(Ptr->Hit(MovedRay, tMin, tMax, rec))
		{
			vec3 n = rec.Normal;
			vec3 nv = RotateXCounterClockWise(Beta, n);		
			rec.Normal = nv;
			
			vec3 a = rec.p - PtrCenter;
			vec3 av = RotateXCounterClockWise(Beta, a);					
			rec.p = av + PtrCenter;
			return true;
		}
		return false;
	}
	
	// NOTE(Alex): We calculate the bbox before any hit call is made,  cause bbox are created on the constructor of bvh node
	// therefore we need to translate bbox for correct aabb testing
	
	virtual bool BBox(float Time0, float Time1, aabb & OutBBox) const
	{
		if(Ptr->BBox(Time0, Time1, OutBBox))
		{
			vec3 Points[8] = {
				vec3(OutBBox.Min.x, OutBBox.Min.y, OutBBox.Min.z),
				vec3(OutBBox.Max.x, OutBBox.Min.y, OutBBox.Min.z),
				vec3(OutBBox.Max.x, OutBBox.Min.y, OutBBox.Max.z),
				vec3(OutBBox.Min.x, OutBBox.Min.y, OutBBox.Max.z),
				vec3(OutBBox.Min.x, OutBBox.Max.y, OutBBox.Min.z),
				vec3(OutBBox.Max.x, OutBBox.Max.y, OutBBox.Min.z),
				vec3(OutBBox.Max.x, OutBBox.Max.y, OutBBox.Max.z),
				vec3(OutBBox.Min.x, OutBBox.Max.y, OutBBox.Max.z),
			};
			
			for(int i = 0; i < 8; i++)
			{
				Points[i] -= PtrCenter;
				Points[i] = RotateXCounterClockWise(Beta, Points[i]);
				Points[i] += PtrCenter;
			}
			
			OutBBox = aabb(Points, ArrayCount(Points));
			return true;
		}
		return false;
	}
	
};

class sphere : public hitable
{
	public:
	vec3 Center;
	float Radius;
	shared_ptr<material> Material;
	
	sphere(vec3 Cen, float r, shared_ptr<material> M) : Center(Cen), Radius(r), Material(M) {}
    
	void GetSphereUV(vec3 p, float * OutU, float * OutV) const
	{
		// NOTE(Alex): returns -pi to pi, when z = 0 return pi / 2 if x i spositive and -pi / 2 if x is negative 
		float Phi = atan2f(p.x, p.z);
		// NOTE(Alex): y goes from 1 to -1 
		// NOTE(Alex): returns 0 to pi radians
		float Theta = acosf(p.y);
		
		*OutU = (Phi < 0) ? (2 * (float)M_PI + Phi) / (2 * (float)M_PI) : Phi / (2 * (float)M_PI);
		*OutV = Theta / (float)M_PI;
	}
	
	virtual bool GetCenter(vec3 & OutCenter) const 
	{ 
		OutCenter = Center;
		return true; 
	}
	
	virtual bool BBox(float Time0, float Time1, aabb & OutBBox) const;
	virtual bool Hit(const ray& r, float tMin, float tMax, hit_record& rec) const;
	
};

bool sphere::BBox(float Time0, float Time1, aabb & OutBBox) const
{
	vec3 R = vec3(Radius, Radius, Radius);
	OutBBox = aabb(Center - R, Center + R);
	return true;
}

bool sphere::Hit(const ray& R, float tMin, float tMax, hit_record& rec) const
{		
	float a = Dot(R.Direction, R.Direction);
	vec3 AMinusC = R.Origin - Center;
	float b = Dot(R.Direction, AMinusC);
	float c = Dot(AMinusC, AMinusC) - Radius * Radius;
	float Radicand = b*b - a*c;	
	
	if(Radicand > 0)
	{
		float t = (-b - sqrt(Radicand)) / (a); 
		if(t > tMin && t < tMax)
		{
			rec.t = t;
			rec.p = R.AtPoint(t);
			rec.Normal = MakeUnitVector(rec.p - Center);
			rec.Material = Material;
			GetSphereUV(MakeUnitVector(rec.p - Center), &rec.u, &rec.v);
			return true;
		}
		
		t = (-b + sqrt(Radicand)) / (a); 
		if(t > tMin && t < tMax)
		{
			rec.t = t;
			rec.p = R.AtPoint(t);
			rec.Normal = MakeUnitVector(rec.p - Center);
			rec.Material = Material;
			GetSphereUV(MakeUnitVector(rec.p - Center), &rec.u, &rec.v);
			return true;
		}
		
	}
	return false;
}

// NOTE(Alex): Motion blur
class moving_sphere : public hitable
{
	public:
	vec3 Center0, Center1;
	float Time0, Time1;
	float Radius;
	shared_ptr<material> Material;
	moving_sphere(vec3 Cen0, vec3 Cen1, float Time0_, float Time1_, float r, shared_ptr<material> M) : 
	Center0(Cen0), Center1(Cen1), Time0(Time0_), Time1(Time1_), Radius(r), Material(M){}
    
	vec3 GetCenterAtTime(const float Time) const
	{
		vec3 Result = (Center1 - Center0) / (Time1 - Time0) * (Time - Time0) + Center0;
		return Result;
	}
    
	virtual bool BBox(float Time0_, float Time1_, aabb & OutBBox) const;
	virtual bool Hit(const ray& r, float tMin, float tMax, hit_record& rec) const;
};

bool moving_sphere::BBox(float Time0_, float Time1_, aabb & OutBBox) const
{
	vec3 R = vec3(Radius, Radius, Radius);
	vec3 C0 = GetCenterAtTime(Time0_);
	vec3 C1 = GetCenterAtTime(Time1_);
	aabb B0 = aabb(C0 - R, C0 + R);
	aabb B1 = aabb(C1 - R, C1 + R);
	OutBBox = UnionAABB(B0, B1);
	return true;
}

bool moving_sphere::Hit(const ray& R, float tMin, float tMax, hit_record& rec) const
{		
	vec3 Center = GetCenterAtTime(R.Time);
	float a = Dot(R.Direction, R.Direction);
	vec3 AMinusC = R.Origin - Center;
	float b = Dot(R.Direction, AMinusC);
	float c = Dot(AMinusC, AMinusC) - Radius * Radius;
	float Radicand = b*b - a*c;	
	
	if(Radicand > 0)
	{
		float t = (-b - sqrt(Radicand)) / (a); 
		if(t > tMin && t < tMax)
		{
			rec.t = t;
			rec.p = R.AtPoint(t);
			rec.Normal = MakeUnitVector(rec.p - Center);
			rec.Material = Material;
			return true;
		}
		
		t = (-b + sqrt(Radicand)) / (a); 
		if(t > tMin && t < tMax)
		{
			rec.t = t;
			rec.p = R.AtPoint(t);
			rec.Normal = MakeUnitVector(rec.p - Center);
			rec.Material = Material;
			return true;
		}	
	}
	
	return false;
}


int CompareXDim(const void * a_, const void * b_)
{
	int Result = 0;
	
	hitable * a = *(hitable**)a_;
	hitable * b = *(hitable**)b_;
	aabb ABBox, BBBox;
	if(!a->BBox(0, 0, ABBox) || !b->BBox(0, 0, BBBox))
	{
		std::cerr << "No bounding box in bvh_node constructor\n";
	}
	
	if((ABBox.Min.X() - BBBox.Min.X()) < 0)
		Result = -1;
	else
		Result = 1;
	
	return Result;
}

int CompareYDim(const void * a_, const void * b_)
{
	int Result = 0;
	
	hitable * a = *(hitable**)a_;
	hitable * b = *(hitable**)b_;
	aabb ABBox, BBBox;
	if(!a->BBox(0, 0, ABBox) || !b->BBox(0, 0, BBBox))
	{
		std::cerr << "No bounding box in bvh_node constructor\n";
	}
	
	if((ABBox.Min.Y() - BBBox.Min.Y()) < 0)
		Result = -1;
	else
		Result = 1;
	
	return Result;
}

int CompareZDim(const void * a_, const void * b_)
{
	int Result = 0;
	
	hitable * a = *(hitable**)a_;
	hitable * b = *(hitable**)b_;
	aabb ABBox, BBBox;
	if(!a->BBox(0, 0, ABBox) || !b->BBox(0, 0, BBBox))
	{
		std::cerr << "No bounding box in bvh_node constructor\n";
	}
	
	if((ABBox.Min.Z() - BBBox.Min.Z()) < 0)
		Result = -1;
	else
		Result = 1;
	
	return Result;
}


class bvh_node : public hitable
{
	private:
	public:
	aabb BBox_;
	shared_ptr<hitable> Left;
	shared_ptr<hitable> Right;
	int Level;
	
	bvh_node();
	bvh_node(vector<shared_ptr<hitable>>::pointer List_, size_t Count, float Time0, float Time1, int Level = 0);
	virtual bool BBox(float Time0, float Time1, aabb & OutBBox) const;
	virtual bool Hit(const ray& r, float tMin, float tMax, hit_record& rec) const;
};

bvh_node::bvh_node(vector<shared_ptr<hitable>>::pointer List_, size_t Count, float Time0, float Time1, int Level_)
{
	// TODO(Alex): sorting algorithm
	if(Count > 0)
	{
		switch(Count)
		{
			case 1:
			{
				Left = Right = List_[0];
			}
			break;
			case 2:
			{
				Left = List_[0];
				Right = List_[1];
			}
			break;
			default:
			{
				vector<shared_ptr<hitable>>::pointer ptr0 = &List_[0];
				vector<shared_ptr<hitable>>::pointer ptr1 = &List_[Count / 2];
				
				Left = make_shared<bvh_node>(ptr0, Count / 2, Time0, Time1, Level_ + 1);
				Right = make_shared<bvh_node>(ptr1, Count - (Count / 2), Time0, Time1, Level_ + 1);
			}
			break;
		}
		
		aabb LBBox, RBBox;
		if(!Left->BBox(Time0, Time1, LBBox) || !Right->BBox(Time0, Time1, RBBox))
		{
			std::cerr << "No bounding box in bvh_node constructor\n";
		}
		
		Level = Level_;		
		BBox_ = UnionAABB(LBBox, RBBox);
	}
}

bool bvh_node::BBox(float Time0, float Time1, aabb & OutBBox) const
{
	OutBBox = BBox_;
	return true;
}

bool bvh_node::Hit(const ray& r, float tMin, float tMax, hit_record& rec) const
{
	bool Result = false;
	if(BBox_.Hit(r, tMin, tMax))
	{
		hit_record LeftRec, RightRec;
		bool LeftHit = Left->Hit(r, tMin, tMax, LeftRec);
		bool RightHit = Right->Hit(r, tMin, tMax, RightRec);
		
		Result = LeftHit || RightHit;
        
		if(LeftHit && RightHit)
		{
			if(LeftRec.t < RightRec.t)
			{
				rec = LeftRec;
			}
			else
			{
				rec = RightRec;
			}
		}
		else if(LeftHit)
		{
			rec = LeftRec;
		}
		else if(RightHit)
		{
			rec = RightRec;			
		}
	}
	return Result;
}




#endif


