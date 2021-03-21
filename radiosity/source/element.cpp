#include "element.h"

void Element::calc_ff(Matrix<float,2>& ffm){
    Matrix<float,1> res{};
    
    Vec3<float> v = MakeUnitVector(n);
    Vec3<float> u = MakeUnitVector(Cross(hm.vup,v));
    
    if(u.norm() == 0)
    {
        // NOTE(Alex): We need to test against other arbitrary vector that is not colinear to v
        u = MakeUnitVector(Cross(hm.other_vup,v));
    }
    
    Vec3<float> w = Cross(v,u);
    
    Vec3<float> TopFaceLowerLeftCorner   = p +  v      + -(1.0f *u + 1.0f*w);  
    Vec3<float> RightFaceLowerLeftCorner = p +  1.0f*u + -1.0f*w;  
    Vec3<float> LeftFaceLowerLeftCorner  = p + -1.0f*u +  1.0f*w;  
    Vec3<float> FrontFaceLowerLeftCorner = p +  1.0f*u +  1.0f*w;  
    Vec3<float> BackFaceLowerLeftCorner  = p + -1.0f*u + -1.0f*w;  
    
    
    // NOTE(Alex): Hemicube´s top face 
    for (size_t y = 0; y < hm.yc; y += 1){
        for (size_t x = 0; x < hm.xc; x += 1){
            float NX = 2.0f*(float(x) / float(hm.xc)) + hm.half_pw;
            float NY = 2.0f*(float(y) / float(hm.yc)) + hm.half_ph;
            
            Vec3<float> RDirection = (TopFaceLowerLeftCorner + NX*u + NY*w) - p;
            Ray R = Ray{p, RDirection};
            
            Element e{space.request_element(R, 0.001f, FLT_MAX)};
            
            if(e.i >= 0)
            {
                float r = RDirection.squared_norm();
                Vec3<float> ij = MakeUnitVector(RDirection);
                Vec3<float> ji = MakeUnitVector(-RDirection);
                float Dotji = dot(e.n,ji);
                
                if(Dotji > 0.0f)
                {
                    float Value{((dot(v, ij) * Dotji * hm.da) / ((float)M_PI  * r * r))};
                    // TODO(Alex): Finish this!!! Make matrix a matrix2D only!!!
                    ffm(i,e.i) += Value;
                }
            }
        }
    }
}


#if 0
template<typename T>
void CalculateFormFactors()
{
    size_t HemiCubeXPixelCount = 100;
    size_t HemiCubeYPixelCount = 100;
    
    size_t HemiCubeHalfYPixelCount = size_t((float)HemiCubeYPixelCount*0.5f);
    float TopFaceSize = 2.0f;
    float DeltaX = TopFaceSize / HemiCubeXPixelCount;
    float DeltaY = TopFaceSize / HemiCubeYPixelCount;
    float HalfPixelWidth = DeltaX * 0.5f;
    float HalfPixelHeight = DeltaY * 0.5f;
    float DeltaA = DeltaX * DeltaY;
    
    vec3 VUP(0,1,0);
    
    // NOTE(Alex): this is for normals that go straight up!!
    vec3 OtherVUP(1,0,0);
    
    // NOTE(Alex): Iterate through all the elements
    for(size_t ElementIndex = 0; ElementIndex < n; ElementIndex += 1)
    {
        vec3 v = MakeUnitVector(Elements[ElementIndex].n);
        vec3 u = MakeUnitVector(Cross(VUP,v));
        
        if(u.Length() == 0)
        {
            // NOTE(Alex): We need to test against other arbitrary vector that is not colinear to v
            u = MakeUnitVector(Cross(OtherVUP,v));
        }
        
        vec3 w = Cross(v,u);
        
        vec3 TopFaceLowerLeftCorner = Elements[ElementIndex].p + v + -(1.0f *u + 1.0f*w);  
        vec3 RightFaceLowerLeftCorner = Elements[ElementIndex].p   +  1.0f*u + -1.0f*w;  
        vec3 LeftFaceLowerLeftCorner = Elements[ElementIndex].p    + -1.0f*u +  1.0f*w;  
        vec3 FrontFaceLowerLeftCorner = Elements[ElementIndex].p   +  1.0f*u +  1.0f*w;  
        vec3 BackFaceLowerLeftCorner = Elements[ElementIndex].p    + -1.0f*u + -1.0f*w;  
        
        // NOTE(Alex): Hemicube´s top face 
        for (size_t y = 0; y < HemiCubeYPixelCount; y += 1){
            for (size_t x = 0; x < HemiCubeXPixelCount; x += 1){
                float NX = 2.0f*(float(x) / float(HemiCubeXPixelCount)) + HalfPixelWidth;
                float NY = 2.0f*(float(y) / float(HemiCubeYPixelCount)) + HalfPixelHeight;
                
                vec3 RDirection = (TopFaceLowerLeftCorner + NX*u + NY*w) - Elements[ElementIndex].p;
                ray R = ray(Elements[ElementIndex].p, RDirection);
                
                radiosity_hit_record JIndexData;
                Hit(R, 0.001f, FLT_MAX, JIndexData);
                
                if(JIndexData.IsValid)
                {
                    float r = RDirection.SquaredLength();
                    vec3 ij = MakeUnitVector(RDirection);
                    vec3 ji = MakeUnitVector(-RDirection);
                    float Dotji = Dot(JIndexData.n,ji);
                    
                    if(Dotji > 0.0f)
                    {
                        T Value(((Dot(v, ij) * Dotji * DeltaA) / ((float)M_PI * r * r)));
                        F[ElementIndex][JIndexData.ElementIndex] += Value;
                    }
                }
            }
        }
        
        // NOTE(Alex): HemicubeÂ´s Right Side face 
        for (size_t y = 0; y < HemiCubeHalfYPixelCount; y += 1){
            for (size_t x = 0; x < HemiCubeXPixelCount; x += 1){
                float NX = 2.0f*(float(x) / float(HemiCubeXPixelCount)) + HalfPixelWidth;
                float NY = 2.0f*(float(y) / float(HemiCubeYPixelCount)) + HalfPixelHeight;
                
                vec3 RDirection = (RightFaceLowerLeftCorner + NX*w + NY*v) - Elements[ElementIndex].p;
                ray R = ray(Elements[ElementIndex].p, RDirection);
                
                radiosity_hit_record JIndexData;
                Hit(R, 0.001f, FLT_MAX, JIndexData);
                if(JIndexData.IsValid)
                {
                    float r = RDirection.SquaredLength();
                    vec3 ij = MakeUnitVector(RDirection);
                    vec3 ji = MakeUnitVector(-RDirection);
                    float Dotji = Dot(JIndexData.n,ji);
                    
                    if (Dotji > 0.0f)
                    {
                        T Value(((Dot(v, ij) * Dotji * DeltaA) / ((float)M_PI * r * r)));
                        F[ElementIndex][JIndexData.ElementIndex] += Value;
                    }
                }
            }
        }
        
        // NOTE(Alex): HemicubeÂ´s Left Side face 
        for (size_t y = 0; y < HemiCubeHalfYPixelCount; y += 1){
            for (size_t x = 0; x < HemiCubeXPixelCount; x += 1){
                float NX = 2.0f*(float(x) / float(HemiCubeXPixelCount)) + HalfPixelWidth;
                float NY = 2.0f*(float(y) / float(HemiCubeYPixelCount)) + HalfPixelHeight;
                
                vec3 RDirection = (LeftFaceLowerLeftCorner + -NX*w + NY*v) - Elements[ElementIndex].p;
                ray R = ray(Elements[ElementIndex].p, RDirection);
                
                radiosity_hit_record JIndexData;
                Hit(R, 0.001f, FLT_MAX, JIndexData);
                if(JIndexData.IsValid)
                {
                    float r = RDirection.SquaredLength();
                    vec3 ij = MakeUnitVector(RDirection);
                    vec3 ji = MakeUnitVector(-RDirection);
                    float Dotji = Dot(JIndexData.n,ji);
                    
                    if (Dotji > 0.0f)
                    {
                        T Value(((Dot(v, ij) * Dotji * DeltaA) / ((float)M_PI * r * r)));
                        F[ElementIndex][JIndexData.ElementIndex] += Value;
                    }
                }
            }
        }
        
        // NOTE(Alex): HemicubeÂ´s Front Side face 
        for (size_t y = 0; y < HemiCubeHalfYPixelCount; y += 1){
            for (size_t x = 0; x < HemiCubeXPixelCount; x += 1){
                float NX = 2.0f*(float(x) / float(HemiCubeXPixelCount)) + HalfPixelWidth;
                float NY = 2.0f*(float(y) / float(HemiCubeYPixelCount)) + HalfPixelHeight;
                
                vec3 RDirection = (FrontFaceLowerLeftCorner + -NX*u + NY*v) - Elements[ElementIndex].p;
                ray R = ray(Elements[ElementIndex].p, RDirection);
                
                radiosity_hit_record JIndexData;
                Hit(R, 0.001f, FLT_MAX, JIndexData);
                if(JIndexData.IsValid)
                {
                    float r = RDirection.SquaredLength();
                    vec3 ij = MakeUnitVector(RDirection);
                    vec3 ji = MakeUnitVector(-RDirection);
                    float Dotji = Dot(JIndexData.n,ji);
                    
                    if (Dotji > 0.0f)
                    {
                        T Value(((Dot(v, ij) * Dotji * DeltaA) / ((float)M_PI * r * r)));
                        F[ElementIndex][JIndexData.ElementIndex] += Value;
                    }
                }
            }
        }
        
        // NOTE(Alex): HemicubeÂ´s Back Side face 
        for (size_t y = 0; y < HemiCubeHalfYPixelCount; y += 1){
            for (size_t x = 0; x < HemiCubeXPixelCount; x += 1){
                float NX = 2.0f*(float(x) / float(HemiCubeXPixelCount)) + HalfPixelWidth;
                float NY = 2.0f*(float(y) / float(HemiCubeYPixelCount)) + HalfPixelHeight;
                
                vec3 RDirection = (BackFaceLowerLeftCorner + NX*u + NY*v) - Elements[ElementIndex].p;
                ray R = ray(Elements[ElementIndex].p, RDirection);
                
                radiosity_hit_record JIndexData;
                Hit(R, 0.001f, FLT_MAX, JIndexData);
                if(JIndexData.IsValid)
                {
                    float r = RDirection.SquaredLength();
                    vec3 ij = MakeUnitVector(RDirection);
                    vec3 ji = MakeUnitVector(-RDirection);
                    float Dotji = Dot(JIndexData.n,ji);
                    
                    if (Dotji > 0.0f)
                    {
                        T Value(((Dot(v,ij) * Dotji * DeltaA) / ((float)M_PI * r * r)));
                        F[ElementIndex][JIndexData.ElementIndex] += Value;
                    }
                }
            }
        }
    }
    
    F.DebugPrint("FormFactor_matrix.ppm");
}
#endif
