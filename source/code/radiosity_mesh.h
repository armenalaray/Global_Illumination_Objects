/* date = February 22nd 2021 5:55 pm */

#ifndef RADIOSITY_MESH_H
#define RADIOSITY_MESH_H

struct element_data
{
    vec3 p;
    vec3 n;
};


class elements_polygon_map
{
    public:
    size_t *ElementIndices;
    size_t PolygonIndex;
    
    elements_polygon_map()
    {
        ElementIndices = 0;
        PolygonIndex = 0;
    }
    
    // TODO(Alex): DO this!!!
#if 0    
    elements_polygon_map(elements_polygon_map & Other)
    {
        ElementIndices = Other.ElementIndices;
        PolygonIndex = Other.PolygonIndex;
    }
    
    elements_polygon_map & operator=(elements_polygon_map & Other)
    {
        ElementIndices = Other.ElementIndices;
        PolygonIndex = Other.PolygonIndex;
        return *this;
    }
#endif
    
    size_t GetIndex()
    {
        if(ElementIndices)
        {
            return ElementIndices[0];
        }
        return 0;
    }
};


template<typename T>
class channel_data
{
    public:
    matrix<T> E; 
    matrix<T> VP; 
    matrix<T> B; 
    
    channel_data(){}
    channel_data(size_t ERowCount, size_t EColumnCount, size_t VPRowCount, size_t VPColumnCount, size_t BRowCount, size_t BColumnCount) : E(ERowCount, EColumnCount), VP(VPRowCount, VPColumnCount), B(BRowCount, BColumnCount){}
};


class radiosity_mesh
{
    public:
    const size_t FaceCount = 5;
    
    size_t HitablePerSide;
    size_t ElementsPerSide;
    size_t n;
    size_t ElementsPerFace;
    
    float FaceWidth;
    float FaceHeight;
    
    element_data * Elements;
    radiosity_hitable ** HitableData;
    elements_polygon_map * Map;
    
    radiosity_vertex_buffer VertexBuffer;
    radiosity_quad_buffer QuadBuffer;
    
    channel_data<radiosity_float> ChannelData[3];
    
    vec3 EmissorCenter;
    
    radiosity_mesh(size_t HitablePerSide_, float FaceWidth_)
    {
        HitablePerSide = HitablePerSide_;
        ElementsPerSide = HitablePerSide;
        ElementsPerFace = ElementsPerSide * ElementsPerSide;
        
        // NOTE(Alex): Emissor element
        n = ElementsPerFace * FaceCount + 1;
        
        FaceWidth = FaceWidth_;
        FaceHeight = FaceWidth;
        
        Elements = 0;
        Map = 0;
        HitableData = 0;
        
        ChannelData[0] = channel_data<radiosity_float>(n,1,n,1,n,1);
        ChannelData[1] = channel_data<radiosity_float>(n,1,n,1,n,1);
        ChannelData[2] = channel_data<radiosity_float>(n,1,n,1,n,1);
        
        EmissorCenter = vec3(FaceWidth_ * 0.5f, FaceWidth_ - 0.1f, FaceWidth_ * 0.5f);
        VertexBuffer = radiosity_vertex_buffer(HitablePerSide_, FaceCount, FaceWidth_, EmissorCenter);
        QuadBuffer = radiosity_quad_buffer(VertexBuffer);
    }
    
    void Hit(ray & r_, float tMin_, float tMax_, radiosity_hit_record & HitRecord)
    {
        float tMax = tMax_;
        radiosity_hit_record HitRecordTemp;
        for(int i = 0; i < n; ++i)
        {
            HitableData[i]->Hit(r_, tMin_, tMax, HitRecordTemp);
            if(HitRecordTemp.IsValid)
            {
                tMax = HitRecordTemp.t;
                HitRecord = HitRecordTemp;
                HitRecord.ElementIndex = Map[i].GetIndex();
                HitRecord.n = Elements[HitRecord.ElementIndex].n;
                HitRecord.Quad = QuadBuffer.Quads[HitRecord.ElementIndex];
            }
        }
    }
    
    void CreateHitables()
    {
        Map = new elements_polygon_map[n];
        size_t HitableIndex = 0;
        HitableData = new radiosity_hitable*[n];
        
        float Step = FaceWidth / float(HitablePerSide);
        
        // NOTE(Alex): XY (0,0) (5,5) Z = 0
        
        {
            float z = 0.0f;
            float y0 = 0;
            float y1 = y0 + Step;
            for (size_t i = 0; i < HitablePerSide ; i += 1, y0 += Step, y1 += Step){
                float x0 = 0; 
                float x1 = x0 + Step; 
                for (size_t j = 0; j < HitablePerSide; 
                     j += 1, 
                     x0 += Step, 
                     x1 += Step, 
                     HitableIndex += 1)
                {
                    HitableData[HitableIndex] = new radiosity_xy_rect(x0, x1, y0, y1, z);
                    Map[HitableIndex].PolygonIndex = HitableIndex;
                }
            }
        }
        
        
        // NOTE(Alex): YZ (0,0) (5,5) X = 0
        
        {
            float x = 0.0f;
            float y0 = 0;
            float y1 = y0 + Step;
            for (size_t i = 0; i < HitablePerSide ; i += 1, y0 += Step, y1 += Step){
                float z0 = 0; 
                float z1 = z0 + Step; 
                for (size_t j = 0; j < HitablePerSide; j += 1, z0 += Step, z1 += Step, HitableIndex += 1){
                    HitableData[HitableIndex] = new radiosity_yz_rect(y0, y1, z0, z1, x);
                    Map[HitableIndex].PolygonIndex = HitableIndex;
                }
            }
        }
        
        // NOTE(Alex): XZ (0,0) (5,5) Y = 0
        
        {
            float y = 0.0f;
            float z0 = 0;
            float z1 = z0 + Step;
            for (size_t i = 0; i < HitablePerSide ; i += 1, z0 += Step, z1 += Step){
                float x0 = 0; 
                float x1 = x0 + Step; 
                for (size_t j = 0; j < HitablePerSide; j += 1, x0 += Step, x1 += Step, HitableIndex += 1){
                    HitableData[HitableIndex] = new radiosity_xz_rect(x0, x1, z0, z1, y);
                    Map[HitableIndex].PolygonIndex = HitableIndex;
                }
            }
        }
        
        // NOTE(Alex): YZ (0,0) (5,5) X = 5.0f
        
        {
            float x = FaceWidth;
            float y0 = 0;
            float y1 = y0 + Step;
            for (size_t i = 0; i < HitablePerSide ; i += 1, y0 += Step, y1 += Step){
                float z0 = 0; 
                float z1 = z0 + Step; 
                for (size_t j = 0; j < HitablePerSide; j += 1, z0 += Step, z1 += Step, HitableIndex += 1){
                    HitableData[HitableIndex] = new radiosity_yz_rect(y0, y1, z0, z1, x);
                    Map[HitableIndex].PolygonIndex = HitableIndex;
                }
            }
        }
        
        // NOTE(Alex): XZ (0,0) (5,5) Y = 5.0f
        
        {
            float y = FaceWidth;
            float z0 = 0;
            float z1 = z0 + Step;
            for (size_t i = 0; i < HitablePerSide ; i += 1, z0 += Step, z1 += Step){
                float x0 = 0; 
                float x1 = x0 + Step; 
                for (size_t j = 0; j < HitablePerSide; j += 1, x0 += Step, x1 += Step, HitableIndex += 1){
                    HitableData[HitableIndex] = new radiosity_xz_rect(x0, x1, z0, z1, y);
                    Map[HitableIndex].PolygonIndex = HitableIndex;
                }
            }
        }
        
        
        // NOTE(Alex): Emissor
        {
            float HalfDiagV = FaceWidth * 0.25f;
            
            float x0 = FaceWidth * 0.5f - HalfDiagV;
            float x1 = FaceWidth * 0.5f + HalfDiagV;
            float z0 = FaceWidth * 0.5f - HalfDiagV;
            float z1 = FaceWidth * 0.5f + HalfDiagV;
            
            HitableData[HitableIndex] = new radiosity_xz_rect(x0, x1, z0, z1, FaceWidth - 0.1f);
            Map[HitableIndex].PolygonIndex = HitableIndex;
        }
    }
    
    void CreateOneElementPerHitable()
    {
        Elements = new element_data[n];
        
        size_t HitableIndex = 0;
        size_t ElementIndex = 0;
        
        float Step = FaceWidth / float(HitablePerSide);
        
        
        // NOTE(Alex): XY (0,0) (5,5) Z = 0
        
        {
            float z = 0.0f;
            float y = Step * 0.5f;
            for (size_t i = 0; i < ElementsPerSide; i += 1, y += Step){
                float x = Step * 0.5f;
                for (size_t j = 0; j < ElementsPerSide; j += 1, x += Step, ElementIndex++, HitableIndex++){
                    Elements[ElementIndex].p = vec3(x, y, z);
                    Elements[ElementIndex].n = vec3(0, 0, 1.0f);
                    Map[HitableIndex].ElementIndices = new size_t[1];
                    Map[HitableIndex].ElementIndices[0] = ElementIndex;
                }
            }
        }
        
        // NOTE(Alex): YZ (0,0) (5,5) X = 0
        
        {
            float x = 0.0f;
            float y = Step * 0.5f;
            for (size_t i = 0; i < ElementsPerSide ; i += 1, y += Step){
                float z = Step * 0.5f;
                for (size_t j = 0; j < ElementsPerSide; j += 1, z += Step, ElementIndex++, HitableIndex++){
                    Elements[ElementIndex].p = vec3(x, y, z);
                    Elements[ElementIndex].n = vec3(1.0f, 0, 0);
                    Map[HitableIndex].ElementIndices = new size_t[1];
                    Map[HitableIndex].ElementIndices[0] = ElementIndex;
                }
            }
        }
        
        // NOTE(Alex): XZ (0,0) (5,5) Y = 0
        
        {
            float y = 0.0f;
            float z = Step * 0.5f;
            for (size_t i = 0; i < ElementsPerSide ; i += 1, z += Step){
                float x = Step * 0.5f;
                for (size_t j = 0; j < ElementsPerSide; j += 1, x += Step, ElementIndex++, HitableIndex++){
                    Elements[ElementIndex].p = vec3(x, y, z);
                    Elements[ElementIndex].n = vec3(0, 1.0f, 0);
                    Map[HitableIndex].ElementIndices = new size_t[1];
                    Map[HitableIndex].ElementIndices[0] = ElementIndex;
                }
            }
        }
        
        // NOTE(Alex): YZ (0,0) (5,5) X = 5.0f
        
        {
            float x = FaceWidth;
            float y = Step * 0.5f;
            for (size_t i = 0; i < ElementsPerSide ; i += 1, y += Step){
                float z = Step * 0.5f;
                for (size_t j = 0; j < ElementsPerSide; j += 1, z += Step, ElementIndex++, HitableIndex++){
                    Elements[ElementIndex].p = vec3(x, y, z);
                    Elements[ElementIndex].n = vec3(-1.0f, 0, 0);
                    Map[HitableIndex].ElementIndices = new size_t[1];
                    Map[HitableIndex].ElementIndices[0] = ElementIndex;
                }
            }
        }
        
        // NOTE(Alex): XZ (0,0) (5,5) Y = 5.0f
        
        {
            float y = FaceWidth;
            float z = Step * 0.5f;
            for (size_t i = 0; i < ElementsPerSide ; i += 1, z += Step){
                float x = Step * 0.5f;
                for (size_t j = 0; j < ElementsPerSide; j += 1, x += Step, ElementIndex++, HitableIndex++){
                    Elements[ElementIndex].p = vec3(x, y, z);
                    Elements[ElementIndex].n = vec3(0, -1.0f, 0);
                    Map[HitableIndex].ElementIndices = new size_t[1];
                    Map[HitableIndex].ElementIndices[0] = ElementIndex;
                }
            }
        }
        
        // NOTE(Alex): Emissor
        {
            Elements[ElementIndex].p = EmissorCenter;
            Elements[ElementIndex].n = vec3(0, -1.0f, 0);
            Map[HitableIndex].ElementIndices = new size_t[1];
            Map[HitableIndex].ElementIndices[0] = ElementIndex;
        }
        
    }
    
    template<typename T>
        void CalculateFormFactors(matrix<T> & F)
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
            
            // NOTE(Alex): Hemicube´s Right Side face 
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
            
            // NOTE(Alex): Hemicube´s Left Side face 
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
            
            // NOTE(Alex): Hemicube´s Front Side face 
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
            
            // NOTE(Alex): Hemicube´s Back Side face 
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
    
    void CalculateRadiosities()
    {
        for (size_t FaceIndex = 0; FaceIndex < FaceCount; FaceIndex += 1){
            for(size_t i = 0; i < HitablePerSide; i += 1){
                for(size_t j = 0; j < HitablePerSide; j += 1){
                    size_t Index = FaceIndex * ElementsPerFace + i * ElementsPerSide + j;
                    
                    if(FaceIndex == 0)
                    {
                        ChannelData[0].VP[Index][0] = 0.73f;
                        ChannelData[1].VP[Index][0] = 0.73f;
                        ChannelData[2].VP[Index][0] = 0.73f;
                    }
                    else if(FaceIndex == 1)
                    {
                        ChannelData[0].VP[Index][0] = 0.12f;
                        ChannelData[1].VP[Index][0] = 0.45f;
                        ChannelData[2].VP[Index][0] = 0.15f;
                    }
                    else if(FaceIndex == 2)
                    {
                        
                        ChannelData[0].VP[Index][0] = 0.73f;
                        ChannelData[1].VP[Index][0] = 0.73f;
                        ChannelData[2].VP[Index][0] = 0.73f;
                    }
                    else if(FaceIndex == 3)
                    {
                        ChannelData[0].VP[Index][0] = 0.65f;
                        ChannelData[1].VP[Index][0] = 0.05f;
                        ChannelData[2].VP[Index][0] = 0.05f;
                    }
                    else if(FaceIndex == 4)
                    {
                        ChannelData[0].VP[Index][0] = 0.73f;
                        ChannelData[1].VP[Index][0] = 0.73f;
                        ChannelData[2].VP[Index][0] = 0.73f;
                    }
                }
            }
        }
        
        ChannelData[0].E[n-1][0] = 15.0f;
        ChannelData[1].E[n-1][0] = 15.0f;
        ChannelData[2].E[n-1][0] = 15.0f;
        
        matrix<radiosity_float> F(n,n);
        
        CreateHitables();
        CreateOneElementPerHitable();
        CalculateFormFactors(F);
        
        matrix<radiosity_float> K(n,n);
        matrix<radiosity_float> M(n,n);
        M.MakeDiagonal();
        
        
        for(int ChannelIndex = 0; ChannelIndex < 3; ChannelIndex += 1)
        {
            matrix<radiosity_float> & VP = ChannelData[ChannelIndex].VP;
            matrix<radiosity_float> & E = ChannelData[ChannelIndex].E;
            matrix<radiosity_float> & B = ChannelData[ChannelIndex].B;
            
            matrix<radiosity_float> Residual(n,1);
            
            matrix<radiosity_float> P(n,n);
            P.MakeDiagonal(VP);
            
            string PString = "P" + to_string(ChannelIndex) + "_matrix.ppm";
            P.DebugPrint(PString);
            
            K = M - P * F;
            
            string KString = "K" + to_string(ChannelIndex) + "_matrix.ppm";
            K.DebugPrint(KString);
            
            for(;;)
            {
                Residual = E - K * B;
                string ResidualString = "Residual" + to_string(ChannelIndex) + "_matrix.ppm";
                Residual.DebugPrint(ResidualString);
                
                radiosity_float Norm = Residual.SquaredNorm();
                if(Norm.Value < 0.1f)
                {
                    break;
                }
                NumSolverGS(K, B, E);
                
                string BString = "B" + to_string(ChannelIndex) + "_matrix.ppm";
                ChannelData[ChannelIndex].B.DebugPrint(BString);
            }
        }
        
        RenderLightMaps();
    }
    
    void RenderLightMaps()
    {
        for (size_t ElementIndex = 0; ElementIndex < n; ElementIndex += 1){
            float r = ChannelData[0].B[ElementIndex][0].Value;
            float g = ChannelData[1].B[ElementIndex][0].Value;
            float b = ChannelData[2].B[ElementIndex][0].Value;
            
            QuadBuffer.Quads[ElementIndex].Vertices[0]->c += vec3(r,g,b);
            QuadBuffer.Quads[ElementIndex].Vertices[1]->c += vec3(r,g,b);
            QuadBuffer.Quads[ElementIndex].Vertices[2]->c += vec3(r,g,b);
            QuadBuffer.Quads[ElementIndex].Vertices[3]->c += vec3(r,g,b);
        }
        
        for (size_t k = 0; k < VertexBuffer.FaceCount; k += 1){
            for (size_t i = 0; i < VertexBuffer.Faces[k].RowCount; i += 1){
                for (size_t j = 0; j < VertexBuffer.Faces[k].ColumnCount; j += 1){
                    size_t LowEdgeX = 0;
                    size_t LowEdgeY = 0;
                    size_t HighEdgeY = VertexBuffer.Faces[k].RowCount - 1;
                    size_t HighEdgeX = VertexBuffer.Faces[k].ColumnCount - 1;
                    
                    if(
                       !((i == LowEdgeY && j == LowEdgeX) || 
                         (i == LowEdgeY && j == HighEdgeX) || 
                         (i == HighEdgeY && j == LowEdgeX) || 
                         (i == HighEdgeY && j == HighEdgeX))
                       )
                    {
                        
                        if(i == LowEdgeY || i == HighEdgeY || j == LowEdgeX || j == HighEdgeX)
                        {
                            VertexBuffer.Faces[k].Data[i][j].c /= 2.0f;
                        }
                        else
                        {
                            VertexBuffer.Faces[k].Data[i][j].c /= 4.0f;
                        }
                    }
                }
            }
            string FileName = "VertexColors_Face" + to_string(k) + ".ppm";
            VertexBuffer.Faces[k].DebugPrint(FileName);
        }
        
        string FileName = "VertexColors_Face" + to_string(FaceCount) + ".ppm";
        VertexBuffer.Faces[FaceCount].DebugPrint(FileName);
        
        
        int TextWidth = 1024;
        int TextHeight = 1024;
        
        float CanvasGap = 0.4f;
        
        // NOTE(Alex): XY (0,0) (5,5) Z = 0
        
        {
            size_t FaceID = 0;
            size_t RowCount = VertexBuffer.Faces[FaceID].RowCount;
            size_t ColumnCount = VertexBuffer.Faces[FaceID].ColumnCount;
            
            vec3 v0 = VertexBuffer.Faces[FaceID].Data[0][0].v;
            vec3 Origin = v0;
            
            vec3 v1 = VertexBuffer.Faces[FaceID].Data[0][ColumnCount - 1].v;
            vec3 v2 = VertexBuffer.Faces[FaceID].Data[RowCount - 1][0].v;
            
            v0 -= Origin;
            v1 -= Origin;
            v2 -= Origin;
            
            vec3 X = v1 - v0;
            vec3 Y = v2 - v0;
            
            X.MakeUnitVector();
            Y.MakeUnitVector();
            
            vec3 SamplingDirection = -Elements[FaceID * ElementsPerFace].n;
            
            ofstream FFMatrix;
            FFMatrix.open("XY_Z0.ppm", ios::trunc | ios::out);
            if(FFMatrix.is_open())
            {
                FFMatrix << "P3\n" << TextWidth << " " << TextHeight << "\n255\n";
                
                for(int PixY = TextHeight - 1; PixY >= 0; --PixY)
                {
                    for(int PixX = 0; PixX < TextWidth; ++PixX)
                    {
                        float u = ((float)PixX / (float)TextWidth); 
                        float v = ((float)PixY / (float)TextHeight);
                        
                        vec3 px = (u * (FaceWidth - CanvasGap)) * X;
                        vec3 py = (v * (FaceHeight - CanvasGap)) * Y;
                        
                        vec3 ShiftX = (CanvasGap * 0.5f) * X;
                        vec3 ShiftY = (CanvasGap * 0.5f) * Y;
                        
                        vec3 p = px + py + Origin + ShiftX + ShiftY;
                        
                        vec3 RDirection = SamplingDirection;
                        ray R = ray(p - RDirection, RDirection);
                        radiosity_hit_record Record;
                        Hit(R, 0.001f, FLT_MAX, Record);
                        if(Record.IsValid)
                        {
                            quad Quad = Record.Quad;
                            
                            float tx = Record.u;
                            float ty = Record.v;
                            
                            vec3 c0 = Quad.Vertices[0]->c;
                            vec3 c1 = Quad.Vertices[1]->c;
                            vec3 c2 = Quad.Vertices[2]->c;
                            vec3 c3 = Quad.Vertices[3]->c;
                            
                            vec3 a = Lerp(c0,c1, tx);
                            vec3 b = Lerp(c2,c3, tx);
                            vec3 c = Lerp(a,b,ty);
                            
                            int ir = int(255.99 * c.r);
                            int ig = int(255.99 * c.g);
                            int ib = int(255.99 * c.b);
                            FFMatrix << ir << " " << ig << " " << ib << endl;
                        }
                    }
                }
                
                FFMatrix.close();
            }
            else cout << "Unable to open file" << endl;
        }
        
        // NOTE(Alex): YZ (0,0) (5,5) X = 0
        
        {
            size_t FaceID = 1;
            size_t RowCount = VertexBuffer.Faces[FaceID].RowCount;
            size_t ColumnCount = VertexBuffer.Faces[FaceID].ColumnCount;
            
            vec3 v0 = VertexBuffer.Faces[FaceID].Data[0][ColumnCount - 1].v;
            vec3 Origin = v0;
            
            vec3 v1 = VertexBuffer.Faces[FaceID].Data[0][0].v;
            vec3 v2 = VertexBuffer.Faces[FaceID].Data[RowCount - 1][ColumnCount - 1].v;
            
            v0 -= Origin;
            v1 -= Origin;
            v2 -= Origin;
            
            vec3 X = v1 - v0;
            vec3 Y = v2 - v0;
            
            X.MakeUnitVector();
            Y.MakeUnitVector();
            
            vec3 SampleDirection = -Elements[FaceID * ElementsPerFace].n;
            
            ofstream FFMatrix;
            FFMatrix.open("YZ_X0.ppm", ios::trunc | ios::out);
            if(FFMatrix.is_open())
            {
                FFMatrix << "P3\n" << TextWidth << " " << TextHeight << "\n255\n";
                
                for(int PixY = TextHeight - 1; PixY >= 0; --PixY)
                {
                    for(int PixX = 0; PixX < TextWidth; ++PixX)
                    {
                        float u = ((float)PixX / (float)TextWidth); 
                        float v = ((float)PixY / (float)TextHeight);
                        
                        vec3 px = (u * (FaceWidth - CanvasGap)) * X;
                        vec3 py = (v * (FaceHeight - CanvasGap)) * Y;
                        
                        vec3 ShiftX = (CanvasGap * 0.5f) * X;
                        vec3 ShiftY = (CanvasGap * 0.5f) * Y;
                        
                        vec3 p = px + py + Origin + ShiftX + ShiftY;
                        
                        vec3 RDirection = SampleDirection;
                        ray R = ray(p - RDirection, RDirection);
                        radiosity_hit_record Record;
                        Hit(R, 0.001f, FLT_MAX, Record);
                        if(Record.IsValid)
                        {
                            quad Quad = Record.Quad;
                            
                            float tx = 1.0f - Record.u;
                            float ty = Record.v;
                            
                            // NOTE(Alex): In this way the parameterization u,v is CORRECT!!!
                            vec3 c0 = Quad.Vertices[1]->c;
                            vec3 c1 = Quad.Vertices[0]->c;
                            vec3 c2 = Quad.Vertices[3]->c;
                            vec3 c3 = Quad.Vertices[2]->c;
                            
                            vec3 a = Lerp(c0,c1, tx);
                            vec3 b = Lerp(c2,c3, tx);
                            vec3 c = Lerp(a,b,ty);
                            
                            int ir = int(255.99 * c.r);
                            int ig = int(255.99 * c.g);
                            int ib = int(255.99 * c.b);
                            FFMatrix << ir << " " << ig << " " << ib << endl;
                        }
                    }
                }
                
                FFMatrix.close();
            }
            else cout << "Unable to open file" << endl;
        }
        
        
        // NOTE(Alex): XZ (0,0) (5,5) Y = 0
        
        {
            size_t FaceID = 2;
            size_t RowCount = VertexBuffer.Faces[FaceID].RowCount;
            size_t ColumnCount = VertexBuffer.Faces[FaceID].ColumnCount;
            
            vec3 v0 = VertexBuffer.Faces[FaceID].Data[RowCount - 1][ColumnCount - 1].v;
            vec3 Origin = v0;
            
            vec3 v1 = VertexBuffer.Faces[FaceID].Data[0][ColumnCount - 1].v;
            vec3 v2 = VertexBuffer.Faces[FaceID].Data[RowCount - 1][0].v;
            
            v0 -= Origin;
            v1 -= Origin;
            v2 -= Origin;
            
            vec3 X = v1 - v0;
            vec3 Y = v2 - v0;
            
            X.MakeUnitVector();
            Y.MakeUnitVector();
            
            vec3 SampleDirection = -Elements[FaceID * ElementsPerFace].n;
            
            ofstream FFMatrix;
            FFMatrix.open("XZ_Y0.ppm", ios::trunc | ios::out);
            if(FFMatrix.is_open())
            {
                FFMatrix << "P3\n" << TextWidth << " " << TextHeight << "\n255\n";
                
                for(int PixY = TextHeight - 1; PixY >= 0; --PixY)
                {
                    for(int PixX = 0; PixX < TextWidth; ++PixX)
                    {
                        float u = ((float)PixX / (float)TextWidth); 
                        float v = ((float)PixY / (float)TextHeight);
                        
                        vec3 px = (u * (FaceWidth - CanvasGap)) * X;
                        vec3 py = (v * (FaceHeight - CanvasGap)) * Y;
                        
                        vec3 ShiftX = (CanvasGap * 0.5f) * X;
                        vec3 ShiftY = (CanvasGap * 0.5f) * Y;
                        
                        vec3 p = px + py + Origin + ShiftX + ShiftY;
                        
                        vec3 RDirection = SampleDirection;
                        ray R = ray(p - RDirection, RDirection);
                        radiosity_hit_record Record;
                        Hit(R, 0.001f, FLT_MAX, Record);
                        if(Record.IsValid)
                        {
                            quad Quad = Record.Quad;
                            
                            float tx = 1.0f - Record.u;
                            float ty = 1.0f - Record.v;
                            
                            // NOTE(Alex): In this way the parameterization u,v is CORRECT!!!
                            vec3 c0 = Quad.Vertices[3]->c;
                            vec3 c1 = Quad.Vertices[1]->c;
                            vec3 c2 = Quad.Vertices[2]->c;
                            vec3 c3 = Quad.Vertices[0]->c;
                            
                            vec3 a = Lerp(c0,c1, tx);
                            vec3 b = Lerp(c2,c3, tx);
                            vec3 c = Lerp(a,b,ty);
                            
                            int ir = int(255.99 * c.r);
                            int ig = int(255.99 * c.g);
                            int ib = int(255.99 * c.b);
                            FFMatrix << ir << " " << ig << " " << ib << endl;
                        }
                    }
                }
                
                FFMatrix.close();
            }
            else cout << "Unable to open file" << endl;
        }
        
        // NOTE(Alex): YZ (0,0) (5,5) X = 5.0f
        
        {
            size_t FaceID = 3;
            size_t RowCount = VertexBuffer.Faces[FaceID].RowCount;
            size_t ColumnCount = VertexBuffer.Faces[FaceID].ColumnCount;
            
            vec3 v0 = VertexBuffer.Faces[FaceID].Data[0][0].v;
            vec3 Origin = v0;
            
            vec3 v1 = VertexBuffer.Faces[FaceID].Data[0][ColumnCount - 1].v;
            vec3 v2 = VertexBuffer.Faces[FaceID].Data[RowCount - 1][0].v;
            
            v0 -= Origin;
            v1 -= Origin;
            v2 -= Origin;
            
            vec3 X = v1 - v0;
            vec3 Y = v2 - v0;
            
            X.MakeUnitVector();
            Y.MakeUnitVector();
            
            vec3 SampleDirection = -Elements[FaceID * ElementsPerFace].n;
            
            ofstream FFMatrix;
            FFMatrix.open("YZ_X5.ppm", ios::trunc | ios::out);
            if(FFMatrix.is_open())
            {
                FFMatrix << "P3\n" << TextWidth << " " << TextHeight << "\n255\n";
                
                for(int PixY = TextHeight - 1; PixY >= 0; --PixY)
                {
                    for(int PixX = 0; PixX < TextWidth; ++PixX)
                    {
                        float u = ((float)PixX / (float)TextWidth); 
                        float v = ((float)PixY / (float)TextHeight);
                        
                        vec3 px = (u * (FaceWidth - CanvasGap)) * X;
                        vec3 py = (v * (FaceHeight - CanvasGap)) * Y;
                        
                        vec3 ShiftX = (CanvasGap * 0.5f) * X;
                        vec3 ShiftY = (CanvasGap * 0.5f) * Y;
                        
                        vec3 p = px + py + Origin + ShiftX + ShiftY;
                        
                        vec3 RDirection = SampleDirection;
                        ray R = ray(p - RDirection, RDirection);
                        radiosity_hit_record Record;
                        Hit(R, 0.001f, FLT_MAX, Record);
                        if(Record.IsValid)
                        {
                            quad Quad = Record.Quad;
                            
                            // NOTE(Alex): I will change it here!!!
                            
                            float tx = Record.u;
                            float ty = Record.v;
                            
                            // NOTE(Alex): In this way the parameterization u,v is CORRECT!!!
                            vec3 c0 = Quad.Vertices[0]->c;
                            vec3 c1 = Quad.Vertices[1]->c;
                            vec3 c2 = Quad.Vertices[2]->c;
                            vec3 c3 = Quad.Vertices[3]->c;
                            
                            vec3 a = Lerp(c0,c1, tx);
                            vec3 b = Lerp(c2,c3, tx);
                            vec3 c = Lerp(a,b,ty);
                            
                            int ir = int(255.99 * c.r);
                            int ig = int(255.99 * c.g);
                            int ib = int(255.99 * c.b);
                            FFMatrix << ir << " " << ig << " " << ib << endl;
                        }
                    }
                }
                
                FFMatrix.close();
            }
            else cout << "Unable to open file" << endl;
        }
        
        
        // NOTE(Alex): XZ (0,0) (5,5) Y = 5.0f
        
        
        {
            size_t FaceID = 4;
            size_t RowCount = VertexBuffer.Faces[FaceID].RowCount;
            size_t ColumnCount = VertexBuffer.Faces[FaceID].ColumnCount;
            
            vec3 v0 = VertexBuffer.Faces[FaceID].Data[0][ColumnCount - 1].v;
            vec3 Origin = v0;
            
            vec3 v1 = VertexBuffer.Faces[FaceID].Data[RowCount - 1][ColumnCount - 1].v;
            vec3 v2 = VertexBuffer.Faces[FaceID].Data[0][0].v;
            
            v0 -= Origin;
            v1 -= Origin;
            v2 -= Origin;
            
            vec3 X = v1 - v0;
            vec3 Y = v2 - v0;
            
            X.MakeUnitVector();
            Y.MakeUnitVector();
            
            vec3 SampleDirection = -Elements[FaceID * ElementsPerFace].n;
            
            ofstream FFMatrix;
            FFMatrix.open("XZ_Y5.ppm", ios::trunc | ios::out);
            if(FFMatrix.is_open())
            {
                FFMatrix << "P3\n" << TextWidth << " " << TextHeight << "\n255\n";
                
                for(int PixY = TextHeight - 1; PixY >= 0; --PixY)
                {
                    for(int PixX = 0; PixX < TextWidth; ++PixX)
                    {
                        float u = ((float)PixX / (float)TextWidth); 
                        float v = ((float)PixY / (float)TextHeight);
                        
                        vec3 px = (u * (FaceWidth - CanvasGap)) * X;
                        vec3 py = (v * (FaceHeight - CanvasGap)) * Y;
                        
                        vec3 ShiftX = (CanvasGap * 0.5f) * X;
                        vec3 ShiftY = (CanvasGap * 0.5f) * Y;
                        
                        vec3 p = px + py + Origin + ShiftX + ShiftY;
                        
                        vec3 RDirection = SampleDirection;
                        ray R = ray(p - RDirection, RDirection);
                        radiosity_hit_record Record;
                        Hit(R, 0.001f, FLT_MAX, Record);
                        if(Record.IsValid)
                        {
                            quad Quad = Record.Quad;
                            
                            float tx = Record.u;
                            float ty = 1.0f - Record.v;
                            
                            // NOTE(Alex): In this way the parameterization u,v is CORRECT!!!
                            vec3 c0 = Quad.Vertices[1]->c;
                            vec3 c1 = Quad.Vertices[3]->c;
                            vec3 c2 = Quad.Vertices[0]->c;
                            vec3 c3 = Quad.Vertices[2]->c;
                            
                            vec3 a = Lerp(c0,c1, tx);
                            vec3 b = Lerp(c2,c3, tx);
                            vec3 c = Lerp(a,b,ty);
                            
                            int ir = int(255.99 * c.r);
                            int ig = int(255.99 * c.g);
                            int ib = int(255.99 * c.b);
                            FFMatrix << ir << " " << ig << " " << ib << endl;
                        }
                    }
                }
                
                FFMatrix.close();
            }
            else cout << "Unable to open file" << endl;
        }
        
    } // NOTE(Alex): End void RenderLightMaps()
};


#endif //RADIOSITY_MESH_H
