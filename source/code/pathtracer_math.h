//////////////////////////////////////////////////////
//
//Author: Alejandro Armenta
//Date: 2019
//File: pathtracer_math.h
//Company:
//
//(2018-2019) All rights reserved.
//
//////////////////////////////////////////////////////

#ifndef PATHTRACER_MATH_H
#define PATHTRACER_MATH_H

template<typename T = float>
inline T ClampTop(const T Top, T Value)
{
	if(Value > Top) return Top; else return Value; 
}

template<typename T = float> 
inline T Lerp(const T &lo, const T &hi, const float t) 
{ 
    return lo * (1 - t) + hi * t; 
}


static int 
RoundToMinusInf(float x)
{
	int Result = (int)x - (x < 0 && x != (int)x);
	return Result;
}

inline float MinF(float A, float B)
{
	bool IsNANA = isnan(A);
	bool IsNANB = isnan(B);
	if(IsNANA && IsNANB) 
		return 0.0f;
	else if(IsNANA) 
		return B;
	else if(IsNANB) 
		return A;
	else 
		return  A < B ? A : B;
}


inline float MaxF(float A, float B)
{
	bool IsNANA = isnan(A);
	bool IsNANB = isnan(B);
	if(IsNANA && IsNANB) 
		return 0.0f;
	else if(IsNANA) 
		return B;
	else if(IsNANB) 
		return A;
	else 
		return  A > B ? A : B;
}


//////////////////////////////////////////////////////////////////////////////////////
// vec2 class
//////////////////////////////////////////////////////////////////////////////////////

template<typename T>
class vec2
{
	public:
	union 
	{
		struct
		{
			T x; 
			T y;
		};
		struct
		{
			T r; 
			T g;
		};
		T e[2];
	};
	
	vec2() : x(T(0)), y(T(0)) {}
	vec2(T x_, T y_) : x(x_), y(y_) {}
	
	vec2 operator *(const T &s) const 
	{
		return vec2(x * s, y * s);
	} 
    
	inline const vec2& operator+() const { return *this; }
	inline vec2 operator-() const { return vec2(-x, -y); }
    
	inline float operator[] (int i) const { return e[i]; }
	inline float& operator[] (int i) { return e[i];}
	
	inline vec2& operator+=(const vec2& v2);
	inline vec2& operator-=(const vec2& v2);
	
	inline vec2& operator*=(const vec2& v2);	
	inline vec2& operator/=(const vec2& v2);
	inline vec2& operator*=(const float t);
	inline vec2& operator/=(const float t);
	
	inline float Length() const 
	{
		return sqrt(SquaredLength());
	}
	
	inline float SquaredLength() const 
	{
		return (x*x + y*y);
	}
	
	inline void MakeUnitVector();
};
typedef vec2<float> vec2f;

template<typename T>
inline istream& operator>>(istream& is, vec2<T>& t)
{
	is >> x >> y ;
	return is;
}

template<typename T>
inline ostream& operator<<(ostream& os, vec2<T>& t)
{
	os << x << y;
	return os;
}

template<typename T>
inline void vec2<T>::MakeUnitVector()
{
    if(Length() != 0)
    {
        float k = 1.0f / vec2::Length();
        x *= k; y *= k;
    }
}

template<typename T>
inline vec2<T> operator+(const vec2<T> & v1, const vec2<T> & v2)
{
	return vec2<T>(v1.e[0] + v2.e[0], v1.e[1] + v2.e[1]);
}

template<typename T>
inline vec2<T> operator-(const vec2<T> & v1, const vec2<T> & v2)
{
	return vec2<T>(v1.e[0] - v2.e[0], v1.e[1] - v2.e[1]);
}

template<typename T>
inline vec2<T> operator*(const vec2<T> & v1, const vec2<T> & v2)
{
	return vec2<T>(v1.e[0] * v2.e[0], v1.e[1] * v2.e[1]);
}

template<typename T>
inline vec2<T> operator/(const vec2<T> & v1, const vec2<T> & v2)
{
	return vec2<T>(v1.e[0] / v2.e[0], v1.e[1] / v2.e[1]);
}

template<typename T>
inline vec2<T> operator*(const vec2<T> & v, float t)
{
	return vec2<T>(v.e[0] * t, v.e[1] * t);
}

template<typename T>
inline vec2<T> operator*(float t, const vec2<T> & v)
{
	return (v * t); 
}

template<typename T>
inline vec2<T> operator/(vec2<T> & v, float t)
{
	return vec2<T>(v.e[0] / t, v.e[1] / t);
}

template<typename T>
inline vec2<T> operator/(float t, vec2<T> & v)
{
	return (v / t);
}

template<typename T>
inline float Dot(const vec2<T> &v1, const vec2<T> & v2)
{
	return v1.e[0] * v2.e[0] + v1.e[1] * v2.e[1]; 
}

template<typename T>
inline vec2<T>& vec2<T>::operator+=(const vec2<T>& v)
{
	e[0] += v.e[0];
	e[1] += v.e[1];	
	return *this;
}


template<typename T>
inline vec2<T>& vec2<T>::operator*=(const vec2<T>& v)
{
	e[0] *= v.e[0];
	e[1] *= v.e[1];
	return *this;
}

template<typename T>
inline vec2<T>& vec2<T>::operator/=(const vec2<T>& v)
{
	e[0] /= v.e[0];
	e[1] /= v.e[1];
	return *this;
}

template<typename T>
inline vec2<T>& vec2<T>::operator-=(const vec2<T>& v)
{
	e[0] -= v.e[0];
	e[1] -= v.e[1];
	return *this;
}

template<typename T>
inline vec2<T>& vec2<T>::operator*=(const float t)
{
	e[0] *= t;
	e[1] *= t;
	return *this;
}

template<typename T>
inline vec2<T>& vec2<T>::operator/=(const float t)
{
	e[0] /= t;
	e[1] /= t;
	return *this;
}

template<typename T>
inline vec2<T> MakeUnitVector(vec2<T> v)
{
    if(v.Length() == 0)
    {
        return v;
    }
    else
    {
        return v / v.Length();
    }
}

// TODO(Alex): Templatize this!
//////////////////////////////////////////////////////////////////////////////////////
// vec3 class
//////////////////////////////////////////////////////////////////////////////////////

class vec3
{
	public:
	union 
	{
		struct
		{
			float x; 
			float y;
			float z;
		};
		struct
		{
			float r; 
			float g;
			float b;
		};
		float e[3];
	};	
	
	// NOTE(Alex): You cannot declare constructors const
	vec3() 
	{		
		e[0] = 0; e[1] = 0; e[2] = 0;
	}
	
	vec3(float e0, float e1, float e2) 
	{ 
		e[0] = e0; e[1] = e1; e[2] = e2;
	}
	
	// NOTE(Alex): A constant memeber function cannot modify any non static data members or call any member functions that aren´t constant.
	inline float X() const { return e[0]; }
	inline float Y() const { return e[1]; }
	inline float Z() const { return e[2]; }
	inline float R() const { return e[0]; }
	inline float G() const { return e[1]; }
	inline float B() const { return e[2]; }
    
	inline const vec3& operator+() const { return *this; }
	inline vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }
    
	inline float operator[] (int i) const { return e[i];}
	inline float& operator[] (int i) { return e[i];}
	
	inline vec3& operator+=(const vec3& v2);
	inline vec3& operator-=(const vec3& v2);
	inline vec3& operator*=(const vec3& v2);
	inline vec3& operator/=(const vec3& v2);
	inline vec3& operator*=(const float t);
	inline vec3& operator/=(const float t);
	
    inline bool operator==(const vec3& v2);
	
	inline float Length() const 
	{
		return sqrt(SquaredLength());
	}
	
	inline float SquaredLength() const 
	{
		return (e[0]*e[0] + e[1]*e[1] + e[2]*e[2]);
	}
	
	inline void MakeUnitVector();
};


inline istream& operator>>(istream& is, vec3& t)
{
	is >> t.e[0] >> t.e[1] >> t.e[2];
	return is;
}

inline ostream& operator<<(ostream& os, vec3& t)
{
	os << t.e[0] << t.e[1] << t.e[2];
	return os;
}

inline void vec3::MakeUnitVector()
{
    if(Length() != 0)
    {
        float k = 1.0f / vec3::Length();
        e[0] *= k; e[1] *= k; e[2] *= k;
    }
}

inline vec3 operator+(const vec3 & v1, const vec3 & v2)
{
	return vec3(v1.e[0] + v2.e[0], v1.e[1] + v2.e[1], v1.e[2] + v2.e[2]);
}

inline vec3 operator-(const vec3 & v1, const vec3 & v2)
{
	return vec3(v1.e[0] - v2.e[0], v1.e[1] - v2.e[1], v1.e[2] - v2.e[2]);
}

inline vec3 operator*(const vec3 & v1, const vec3 & v2)
{
	return vec3(v1.e[0] * v2.e[0], v1.e[1] * v2.e[1], v1.e[2] * v2.e[2]);
}

inline vec3 operator/(const vec3 & v1, const vec3 & v2)
{
	return vec3(v1.e[0] / v2.e[0], v1.e[1] / v2.e[1], v1.e[2] / v2.e[2]);
}

inline vec3 operator*(const vec3 & v, float t)
{
	return vec3(v.e[0] * t, v.e[1] * t, v.e[2] * t);
}

inline vec3 operator*(float t, const vec3 & v)
{
	return (v * t); 
}

inline vec3 operator/(vec3 & v, float t)
{
	return vec3(v.e[0] / t, v.e[1] / t, v.e[2] / t);
}

inline vec3 operator/(float t, vec3 & v)
{
	return (v / t);
}


inline float Dot(const vec3 &v1, const vec3 & v2)
{
	return v1.e[0] * v2.e[0] + v1.e[1] * v2.e[1] + v1.e[2] * v2.e[2]; 
}


inline vec3 Cross(const vec3 &v1, const vec3 & v2)
{
	return vec3(
                (v1.e[1] * v2.e[2] - v1.e[2] * v2.e[1]), 
                (-(v1.e[0] * v2.e[2] - v1.e[2] * v2.e[0])),   
                (v1.e[0] * v2.e[1] - v1.e[1] * v2.e[0])
                ); 
}

inline vec3 & vec3::operator+=(const vec3 &v)
{
	e[0] += v.e[0];
	e[1] += v.e[1];
	e[2] += v.e[2];
	
	return *this;
}


inline vec3 & vec3::operator*=(const vec3 &v)
{
	e[0] *= v.e[0];
	e[1] *= v.e[1];
	e[2] *= v.e[2];
	
	return *this;
}

inline vec3 & vec3::operator/=(const vec3 &v)
{
	e[0] /= v.e[0];
	e[1] /= v.e[1];
	e[2] /= v.e[2];
	
	return *this;
}

inline vec3 & vec3::operator-=(const vec3 &v)
{
	e[0] -= v.e[0];
	e[1] -= v.e[1];
	e[2] -= v.e[2];
	
	return *this;
}


inline vec3 & vec3::operator*=(const float t)
{
	e[0] *= t;
	e[1] *= t;
	e[2] *= t;
	
	return *this;
}

inline vec3 & vec3::operator/=(const float t)
{
	e[0] /= t;
	e[1] /= t;
	e[2] /= t;
	
	return *this;
}

inline bool vec3::operator==(const vec3& v2)
{
    bool Result = false;
    float Norm = (*this - v2).Length();
    Result = (Norm < 0.1f) ? true : false;
    return Result;
}

inline vec3 MakeUnitVector(vec3 v)
{
    if(v.Length() == 0)
    {
        return v;
    }
    else
    {
        return v / v.Length();
    }
}

// TODO(Alex): Check this, it should be 
// cos(angle) + sin(Angle)
// -sin(angle) + cos(Angle)
vec3 RotateZCounterClockWise(float Angle, vec3 p)
{
	float length = p.Length();
	p.MakeUnitVector();
	vec3 Result; 
	Result.x = p.x * cos(Angle) - p.y * sin(Angle);
	Result.y = p.x * sin(Angle) + p.y * cos(Angle);
	Result.z = p.z; 
	return Result * length;
}

vec3 RotateYCounterClockWise(float Angle, vec3 p)
{
	float length = p.Length();
	p.MakeUnitVector();
	vec3 Result; 
	Result.z = p.z * cos(Angle) - p.x * sin(Angle);
	Result.x = p.z * sin(Angle) + p.x * cos(Angle);
	Result.y = p.y; 
	return Result * length;
}

vec3 RotateXCounterClockWise(float Angle, vec3 p)
{
	float length = p.Length();
	p.MakeUnitVector();
	vec3 Result; 
	Result.z = p.z * cos(Angle) + p.y * sin(Angle);
	Result.y = -p.z * sin(Angle) + p.y * cos(Angle);
	Result.x = p.x; 
	return Result * length;
}

//////////////////////////////////////////////////////////////////////////////////////
// matrix4x4 class
//////////////////////////////////////////////////////////////////////////////////////

// TODO(Alex): TEST THIS
template<typename T>
class matrix44
{
	public:
	T m[4][4];
	
	matrix44() 
	{
		for(uint8_t i = 0; i < 4; i++)
		{
			for(uint8_t j = 0; j < 4; j++)
			{
				m[i][j] = 0;
			}
		}
	}
	
	matrix44(T e00, 
			 T e01,
			 T e02,
			 T e03,
			 T e04,
			 T e05,
			 T e06,
			 T e07,
			 T e08,
			 T e09,
			 T e10,
			 T e11,
			 T e12,
			 T e13,
			 T e14,
			 T e15
			 ) 
	{
		m[0][0] = e00; 
		m[0][1] = e01;
		m[0][2] = e02;
		m[0][3] = e03;
		m[1][0] = e04;
		m[1][1] = e05;
		m[1][2] = e06;
		m[1][3] = e07;
		m[2][0] = e08;
		m[2][1] = e09;
		m[2][2] = e10;
		m[2][3] = e11;
		m[3][0] = e12;
		m[3][1] = e13;
		m[3][2] = e14;
		m[3][3] = e15;
	}
	
	matrix44 & operator=(matrix44 & Other)
	{
		for(uint8_t i = 0; i < 4; i++)
		{
			for(uint8_t j = 0; j < 4; j++)
			{
				m[i][j] = Other[i][j];
			}
		}
		return *this;
	}
	
	matrix44 Identity(void) 
	{ 
		matrix44 a(1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1);
		return a; 
	}
	
	const T * operator [] (uint8_t i) const 
	{ 
		return m[i]; 
	}
	
	T * operator [] (uint8_t i) 
	{ 
		return m[i]; 
	}
	
	matrix44 operator * (const matrix44 & o) const
	{
		matrix44 Result;
		for (uint8_t i  = 0; i < 4; ++i)
		{
			for (uint8_t j  = 0; j < 4; ++j)
			{
				Result[i][j] = 
					m[i][0] * o[0][j] + 
					m[i][1] * o[1][j] + 
					m[i][2] * o[2][j] + 
					m[i][3] * o[3][j]; 
			}
		}
		
		return Result;
	}
	
	
	vec3 MultP(const vec3 & Src) const
	{
		vec3 Result;
		
		Result.x = Src.x * m[0][0] + Src.y * m[1][0] + Src.z * m[2][0] + m[3][0];
		Result.y = Src.x * m[0][1] + Src.y * m[1][1] + Src.z * m[2][1] + m[3][1];
		Result.z = Src.x * m[0][2] + Src.y * m[1][2] + Src.z * m[2][2] + m[3][2];
		T w      = Src.x * m[0][3] + Src.y * m[1][3] + Src.z * m[2][3] + m[3][3];
		
		if(w != 1 && w != 0)
		{
			Result.x /= w;
			Result.y /= w;
			Result.z /= w;
		}
		
		return Result;
	}
	
	
	vec3 MultV(const vec3 & Src) const
	{
		vec3 Result;
		
		//#define ROWMAJOR 2
#ifdef ROWMAJOR		
		Result.x = Src.x * m[0][0] + Src.y * m[1][0] + Src.z * m[2][0];
		Result.y = Src.x * m[0][1] + Src.y * m[1][1] + Src.z * m[2][1];
		Result.z = Src.x * m[0][2] + Src.y * m[1][2] + Src.z * m[2][2];				
#else
		Result.x = Src.x * m[0][0] + Src.y * m[0][1] + Src.z * m[0][2];
		Result.y = Src.x * m[1][0] + Src.y * m[1][1] + Src.z * m[1][2];
		Result.z = Src.x * m[2][0] + Src.y * m[2][1] + Src.z * m[2][2];				
#endif
		
		return Result;
	}
	
	matrix44 Transpose() const
	{
		matrix44 Result;
		for(uint8_t i = 0; i < 4; i++)
		{
			for(uint8_t j = 0; j < 4; j++)
			{
				Result.m[j][i] = m[i][j];
			}
		}
		return Result;
	}
	
	// NOTE(Alex): THIS IS ALREADY TESTED!	
	bool Invert(matrix44<T> & Out_)
	{
		matrix44<T> Out = Identity();
		matrix44<T> Temp = *this;
		for(uint8_t Column = 0; Column < 4; ++Column)
		{
			if(Temp[Column][Column] == 0)
			{
				uint8_t Big = Column;
				for(uint8_t Row = 0; Row < 4; ++Row)
					if(abs(Temp[Row][Column]) > abs(Temp[Big][Column])) Big = Row;
				
				if(Big == Column) return false;
				else
				{
					for(size_t j = 0; j < 4; ++j)
					{
						std::swap(Temp[Column][j], Temp[Big][j]);
						std::swap(Out[Column][j], Out[Big][j]);
					}
				}
				
			}
			
			for(uint8_t Row = 0; Row < 4; ++Row)
			{
				if(Row != Column && Temp[Row][Column] != 0)
				{
					T d =  Temp[Row][Column] / Temp[Column][Column];
					for(uint8_t j = 0; j < 4; ++j)
					{
						Temp[Row][j] -=  Temp[Column][j] * d; 
						Out[Row][j] -=  Out[Column][j] * d; 
					}
					
					// NOTE(Alex): Set the elment to 0 for safety;
					Temp[Row][Column] = 0;
				}
			}
		}
		
		
		for(uint8_t Row = 0; Row < 4; ++Row)
		{
			T e = Temp[Row][Row];
			for(uint8_t Column = 0; Column < 4; ++Column)
			{
				Out[Row][Column] /= e;
			}
		}
		
		Out_ = Out;
		return true;
	}
	
	friend std::ostream& operator << (std::ostream & S, const matrix44 & m)
	{
		std::ios_base::fmtflags OldFlags = S.flags();
		
		int Width = 12;
		S.precision(5); //Control the number of displayed decimals
		S.setf(std::ios_base::fixed);
		
		S << 
			" " << std::setw(Width) << m[0][0] << 
			" " << std::setw(Width) << m[0][1] << 
			" " << std::setw(Width) << m[0][2] << 
			" " << std::setw(Width) << m[0][3] <<
    		
			"\n" << 
            
			" " << std::setw(Width) << m[1][0] << 
			" " << std::setw(Width) << m[1][1] << 
			" " << std::setw(Width) << m[1][2] << 
			" " << std::setw(Width) << m[1][3] <<
			
			"\n" <<
            
			" " << std::setw(Width) << m[2][0] << 
			" " << std::setw(Width) << m[2][1] << 
			" " << std::setw(Width) << m[2][2] << 
			" " << std::setw(Width) << m[2][3] <<
            
			"\n" <<
            
			" " << std::setw(Width) << m[3][0] << 
			" " << std::setw(Width) << m[3][1] << 
			" " << std::setw(Width) << m[3][2] << 
			" " << std::setw(Width) << m[3][3] <<
            
			"\n";
		
		S.flags(OldFlags);
		return S;
	}
};

typedef matrix44<float> matrix44f;
typedef matrix44<double> matrix44d;


//////////////////////////////////////////////////////////////////////////////////////
// ray class
//////////////////////////////////////////////////////////////////////////////////////


class ray
{
	public:
	
	ray() {}
	ray(const vec3 & Origin_, const vec3 & Direction_, float Time_ = 0.0f) : Origin(Origin_), Direction(Direction_), Time(Time_) {}
	vec3 AtPoint(float t) const { return Origin + t * Direction; }
	
	vec3 Origin;
	vec3 Direction;
	float Time;
};


//////////////////////////////////////////////////////////////////////////////////////
// aabb class
//////////////////////////////////////////////////////////////////////////////////////


class aabb 
{
	public:
	vec3 Min, Max;
	
	aabb() : Min(FLT_MAX,FLT_MAX,FLT_MAX), Max(-FLT_MAX,-FLT_MAX,-FLT_MAX) {};
	aabb(vec3 Min_, vec3 Max_) : Min(Min_), Max(Max_){}
	aabb(vec3 * PA, int Count) : aabb()
	{
		for(int i = 0; i < Count; i++)
		{
			vec3 * v = PA + i;
			if(v->x < Min.x) Min.x = v->x; 
			if(v->y < Min.y) Min.y = v->y; 
			if(v->z < Min.z) Min.z = v->z; 
			
			if(v->x > Max.x) Max.x = v->x; 
			if(v->y > Max.y) Max.y = v->y; 
			if(v->z > Max.z) Max.z = v->z; 
		}
	}
	
    
	inline bool Hit(const ray& r_, float tMin_, float tMax_) const
	{
		float tMin = tMin_;
		float tMax = tMax_;
		for(int i = 0; i < 3; i++)
		{
            
#if 1			
			float t0 = MinF((Min[i] - r_.Origin[i]) / r_.Direction[i], (Max[i] - r_.Origin[i]) / r_.Direction[i]); 
			float t1 = MaxF((Min[i] - r_.Origin[i]) / r_.Direction[i], (Max[i] - r_.Origin[i]) / r_.Direction[i]); 
			tMin = MaxF(t0, tMin);
			tMax = MinF(t1, tMax);
			if(tMax <= tMin) return false;
#else
			float DivD = 1 / r_.Direction[i];
			float t0 = (Min[i] - r_.Origin[i]) * DivD;
			float t1 = (Max[i] - r_.Origin[i]) * DivD;
			if(DivD < 0.0f) std::swap(t0, t1);
			
			tMin = t0 > tMin ? t0 : tMin;
			tMax = t1 < tMax ? t1 : tMax;
			if(tMax <= tMin) return false;
#endif
            
		}
		return true;
	}
};


aabb OverlappedBBox()
{
	aabb Result(vec3(FLT_MAX,FLT_MAX,FLT_MAX),vec3(-FLT_MAX,-FLT_MAX,-FLT_MAX));
	return Result;
}

aabb UnionAABB(const aabb & B0, const aabb & B1)
{
	aabb Result(
                vec3(
                     MinF(B0.Min[0],B1.Min[0]),
                     MinF(B0.Min[1],B1.Min[1]),
                     MinF(B0.Min[2],B1.Min[2])
                     ),
                vec3(
                     MaxF(B0.Max[0],B1.Max[0]),
                     MaxF(B0.Max[1],B1.Max[1]),
                     MaxF(B0.Max[2],B1.Max[2])
                     ));
	
	return Result;
}


#endif


