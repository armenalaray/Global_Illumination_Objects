/* date = February 22nd 2021 0:22 pm */

#ifndef RADIOSITY_HITABLE_H
#define RADIOSITY_HITABLE_H

class radiosity_vertex_color
{
    public:
    vec3 v;
    vec3 c;
    
    radiosity_vertex_color()
    {
    }
    
    
    radiosity_vertex_color(float x, float y, float z)
    {
        v = vec3(x,y,z);
    }
    
    radiosity_vertex_color(vec3 Other)
    {
        v = Other;
    }
    
    radiosity_vertex_color & operator+=(radiosity_vertex_color & Other)
    {
        return *this;
    }
    
    friend std::ostream& operator << (std::ostream & S, radiosity_vertex_color & F)
    {
        return S;
    }
    
    std::string PrintPPMPixel()
    {
        string Result;
        //FFValue /= MaxPixVal;
        int ir = int(255.99 * c.x);
        int ig = int(255.99 * c.y);
        int ib = int(255.99 * c.z);
        
        Result = to_string(ir) + " " + to_string(ig) + " " + to_string(ib);
        return Result;
    }
};

class quad
{
    public:
    radiosity_vertex_color * Vertices[4];
    
    quad()
    {
    }
    
    quad(radiosity_vertex_color * v0_,radiosity_vertex_color * v1_,radiosity_vertex_color * v2_,radiosity_vertex_color * v3_)
    {
        Vertices[0] = v0_;
        Vertices[1] = v1_;
        Vertices[2] = v2_;
        Vertices[3] = v3_;
    }
};


struct radiosity_hit_record
{
    // NOTE(Alex): Components for form factors
    size_t ElementIndex;
    bool IsValid;
    float t;
    vec3 n;
    
    // NOTE(Alex): Components to render
    quad Quad;
    float u;
    float v;
    
    radiosity_hit_record() : n(), Quad()
    {
        IsValid = false;
        ElementIndex = 0;
        t = 0.0f;
        
        u = 0.0f;
        v = 0.0f;
    }
    
    radiosity_hit_record(radiosity_hit_record & Other)
    {
        ElementIndex = Other.ElementIndex;
        IsValid = Other.IsValid;
        t = Other.t;
        n = Other.n;
        
        Quad = Other.Quad;
        u = Other.u;
        v = Other.v;
    }
    
    radiosity_hit_record & operator=(radiosity_hit_record & Other)
    {
        ElementIndex = Other.ElementIndex;
        IsValid = Other.IsValid;
        t = Other.t;
        n = Other.n;
        
        Quad = Other.Quad;
        u = Other.u;
        v = Other.v;
        
        return *this;
    }
};

class radiosity_hitable
{
    public:
    virtual void Hit(ray & r, float tMin, float tMax, radiosity_hit_record & HitRecord) = 0;
};

class radiosity_xy_rect : public radiosity_hitable
{
	public:
	float x0, x1, y0, y1, k;
	
	radiosity_xy_rect(float x0_, float x1_, float y0_, float y1_, float k_) : 
	x0(x0_), 
	x1(x1_), 
	y0(y0_), 
	y1(y1_), 
	k(k_) 
	{}
	
	virtual void Hit(ray & r, float tMin, float tMax, radiosity_hit_record & HitRecord);
};

void radiosity_xy_rect::Hit(ray & r, float tMin, float tMax, radiosity_hit_record & HitRecord)
{
	float t = (k - r.Origin.z) / r.Direction.z;
    
	if(t < tMin || t > tMax) 
    {
        HitRecord.IsValid = false;
        return;
    }
	
	float x = r.Origin.x + t * r.Direction.x;
	float y = r.Origin.y + t * r.Direction.y;
	
	if(x < x0 || x > x1 || y < y0 || y > y1)
    {
        HitRecord.IsValid = false;
        return;
    }
	
    HitRecord.t = t;
    HitRecord.u = ((x - x0) / (x1 - x0));
	HitRecord.v = ((y - y0) / (y1 - y0));
    HitRecord.IsValid = true;
}


class radiosity_xz_rect : public radiosity_hitable
{
	public:
	
	float x0, x1, z0, z1, k;
	
	radiosity_xz_rect(float x0_, float x1_, float z0_, float z1_, float k_) : 
	x0(x0_), 
	x1(x1_), 
	z0(z0_), 
	z1(z1_), 
	k(k_) 
	{}
	
    virtual void Hit(ray & r, float tMin, float tMax, radiosity_hit_record & HitRecord);
};

void radiosity_xz_rect::Hit(ray & r, float tMin, float tMax, radiosity_hit_record & HitRecord)
{
	float t = (k - r.Origin.y) / r.Direction.y;
	
	if(t < tMin || t > tMax)
    {
        HitRecord.IsValid = false;
        return;
    }
	
	float x = r.Origin.x + t * r.Direction.x;
	float z = r.Origin.z + t * r.Direction.z;
	
	if(x < x0 || x > x1 || z < z0 || z > z1)
    {
        HitRecord.IsValid = false; 
        return;
    }
    
	HitRecord.t = t;
	HitRecord.u = ((z - z0) / (z1 - z0));
    HitRecord.v = ((x - x0) / (x1 - x0));
    HitRecord.IsValid = true;
}

class radiosity_yz_rect : public radiosity_hitable
{
	public:
	
	float y0, y1, z0, z1, k;
	
	radiosity_yz_rect(float y0_, float y1_, float z0_, float z1_, float k_) : 
	y0(y0_), 
	y1(y1_), 
	z0(z0_), 
	z1(z1_), 
	k(k_)
	{}
	
    virtual void Hit(ray & r, float tMin, float tMax, radiosity_hit_record & HitRecord);
    
};

void radiosity_yz_rect::Hit(ray & r, float tMin, float tMax, radiosity_hit_record & HitRecord)
{
	float t = (k - r.Origin.x) / r.Direction.x;
	
	if(t < tMin || t > tMax)
    {
        HitRecord.IsValid = false;
        return;
    }
	
	float y = r.Origin.y + t * r.Direction.y;
	float z = r.Origin.z + t * r.Direction.z;
	
	if(y < y0 || y > y1 || z < z0 || z > z1)
    {
        HitRecord.IsValid = false;
        return;
    }
	
	HitRecord.t = t;
	HitRecord.u = ((z - z0) / (z1 - z0));
    HitRecord.v = ((y - y0) / (y1 - y0));
    HitRecord.IsValid = true;
}


class radiosity_hitable_list : public radiosity_hitable
{
	public:
    size_t Count;
    radiosity_hitable ** List;
	
    radiosity_hitable_list(radiosity_hitable ** List_, size_t Count_)
    {
        Count = Count_;
		List = List_;
	}
    
    virtual void Hit(ray & r, float tMin, float tMax, radiosity_hit_record & HitRecord);
};


void radiosity_hitable_list::Hit(ray & r_, float tMin_, float tMax_, radiosity_hit_record & HitRecord)
{
	float tMax = tMax_;
    radiosity_hit_record HitRecordTemp;
	for(int i = 0; i < Count; ++i)
	{
        List[i]->Hit(r_, tMin_, tMax, HitRecordTemp);
		if(HitRecordTemp.IsValid)
		{
            tMax = HitRecordTemp.t;
            HitRecord = HitRecordTemp;
		}
	}
}


#endif //RADIOSITY_HITABLE_H
