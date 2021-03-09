/* date = February 24th 2021 2:48 pm */

#ifndef RADIOSITY_VERTEX_H
#define RADIOSITY_VERTEX_H


/*
Entonces primero genero los quads con los vertices.
No se como!!!

Despues genero elementos interpolando los quads!! u = 0.5 y v = 0.5

Despues genero Hitables sacando bounding box!!

Despues hago todo lo que ya hacía, y hasta ahi voy a hacer por ahora!!!!


*/

class radiosity_vertex_face : public radiosity_bidimensional_array<radiosity_vertex_color>
{
    public:
    radiosity_vertex_face() : radiosity_bidimensional_array<radiosity_vertex_color>() {}
    radiosity_vertex_face(size_t RowCount_, size_t ColumnCount_) : radiosity_bidimensional_array<radiosity_vertex_color>(RowCount_, ColumnCount_) {}
    
    
    void DebugPrint(std::string FileName)
    {
        // TODO(Alex): Do we want bigger pixels???
        ofstream FFMatrix;
        FFMatrix.open(FileName, ios::trunc | ios::out);
        if(FFMatrix.is_open())
        {
            FFMatrix << "P3\n" << ColumnCount << " " << RowCount << "\n255\n";
            
            for(int i = (int)(RowCount - 1); i >= 0; i -= 1)
            {
                for(int j = 0; j < (int) ColumnCount; j += 1)
                {
                    FFMatrix <<  Data[i][j].PrintPPMPixel() << endl;
                }
            }
            
            FFMatrix.close();
        }
        else cout << "Unable to open file" << endl;
        
    }
    
};

class radiosity_vertex_buffer
{
    public:
    size_t HitablePerSide;
    size_t VertexPerSide;
    size_t FaceCount;
    float FaceWidth;
    
    radiosity_vertex_face * Faces;
    vec3 EmissorCenter;
    
    radiosity_vertex_buffer() : EmissorCenter()
    {
        HitablePerSide = 0;
        VertexPerSide = 0;
        FaceCount = 0;
        FaceWidth = 0;
        Faces = 0;
    }
    
    radiosity_vertex_buffer(size_t HitablePerSide_, size_t FaceCount_, float FaceWidth_, vec3 EmissorCenter_)
    {
        EmissorCenter = EmissorCenter_;
        FaceWidth = FaceWidth_;
        HitablePerSide = HitablePerSide_;
        VertexPerSide = HitablePerSide + 1;
        FaceCount = FaceCount_;
        GenerateVertices();
    }
    
    void GenerateVertices()
    {
        size_t FaceIndex = 0;
        // NOTE(Alex): Emissor Face;
        Faces = new radiosity_vertex_face[FaceCount + 1];
        Faces[FaceCount] = radiosity_vertex_face(2,2);
        
        for (size_t i = 0; i < FaceCount; i += 1){
            Faces[i] = radiosity_vertex_face(VertexPerSide,VertexPerSide);
        }
        
        
        float Step = FaceWidth / float(HitablePerSide);
        
        // NOTE(Alex): XY (0,0) (5,5) Z = 0
        {
            
            float z = 0.0f;
            float y = 0.0f;
            for (size_t i = 0; i < VertexPerSide; i += 1, y += Step){
                float x = 0.0f;
                for (size_t j = 0; j < VertexPerSide; j += 1, x += Step){
                    Faces[FaceIndex].Data[i][j] = radiosity_vertex_color(x,y,z);
                }
            }
        }
        
        FaceIndex++;
        
        // NOTE(Alex): YZ (0,0) (5,5) X = 0
        
        {
            float x = 0.0f;
            float y = 0.0f;
            for (size_t i = 0; i < VertexPerSide; i += 1, y += Step){
                float z = 0.0f;
                for (size_t j = 0; j < VertexPerSide; j += 1, z += Step){
                    Faces[FaceIndex].Data[i][j] = radiosity_vertex_color(x,y,z);
                }
            }
        }
        
        FaceIndex++;
        
        // NOTE(Alex): XZ (0,0) (5,5) Y = 0
        
        {
            float y = 0.0f;
            float z = 0.0f;
            for (size_t i = 0; i < VertexPerSide; i += 1, z += Step){
                float x = 0.0f;
                for (size_t j = 0; j < VertexPerSide; j += 1, x += Step){
                    Faces[FaceIndex].Data[i][j] = radiosity_vertex_color(x,y,z);
                }
            }
        }
        
        FaceIndex++;
        
        // NOTE(Alex): YZ (0,0) (5,5) X = 5.0f
        
        {
            float x = FaceWidth;
            float y = 0.0f;
            for (size_t i = 0; i < VertexPerSide; i += 1, y += Step){
                float z = 0.0f;
                for (size_t j = 0; j < VertexPerSide; j += 1, z += Step){
                    Faces[FaceIndex].Data[i][j] = radiosity_vertex_color(x,y,z);
                }
            }
        }
        
        FaceIndex++;
        
        // NOTE(Alex): XZ (0,0) (5,5) Y = 5.0f
        
        {
            float y = FaceWidth;
            float z = 0.0f;
            for (size_t i = 0; i < VertexPerSide; i += 1, z += Step){
                float x = 0.0f;
                for (size_t j = 0; j < VertexPerSide; j += 1, x += Step){
                    Faces[FaceIndex].Data[i][j] = radiosity_vertex_color(x,y,z);
                }
            }
        }
        
        
        /*How do I find the right place to put the Center the emissor*/
        // NOTE(Alex): HitablePerSide has to be even!!
        //size_t EmissorSideCount = size_t(HitablePerSide * 0.5f);
        
        //if(IsOdd(EmissorSideCount))
        //{
        //EmissorSideCount += 1;
        //}
        
        //size_t Gap = HitablePerSide - EmissorSideCount;
        
        //vec2<size_t> FullDiagonal(HitablePerSide - 1 ,HitablePerSide - 1);
        //vec2<size_t> Diagonal = FullDiagonal - vec2<size_t>(Gap,Gap);
        
        //size_t HalfGap = size_t(float(Gap)*0.5f);
        
        //vec2<size_t> EmissorLowBound = vec2<size_t>(HalfGap,HalfGap);
        //vec2<size_t> EmissorHighBound = Diagonal + EmissorLowBound;
        
        //radiosity_aaquad<size_t> EmissorQuad(EmissorLowBound, EmissorHighBound);
        
        // NOTE(Alex): Emissor
        
        
        float HalfDiagV = FaceWidth * 0.25f;
        
        vec3 V0(-HalfDiagV, 0, -HalfDiagV); 
        vec3 V1( HalfDiagV, 0, -HalfDiagV); 
        vec3 V2(-HalfDiagV, 0,  HalfDiagV); 
        vec3 V3( HalfDiagV, 0,  HalfDiagV); 
        
        Faces[FaceCount].Data[0][0] = radiosity_vertex_color(V0 + EmissorCenter);
        Faces[FaceCount].Data[0][1] = radiosity_vertex_color(V1 + EmissorCenter);
        Faces[FaceCount].Data[1][0] = radiosity_vertex_color(V2 + EmissorCenter);
        Faces[FaceCount].Data[1][1] = radiosity_vertex_color(V3 + EmissorCenter);
    }
};


class radiosity_quad_buffer
{
    public:
    vector<quad> Quads;
    
    radiosity_quad_buffer()
    {
    }
    
    radiosity_quad_buffer(radiosity_vertex_buffer VerBuff)
    {
        for (size_t k = 0; k < VerBuff.FaceCount + 1; k += 1){
            for (size_t i = 0; i < VerBuff.Faces[k].RowCount - 1; i += 1){
                for (size_t j = 0; j < VerBuff.Faces[k].ColumnCount - 1; j += 1){
                    radiosity_vertex_color * v0 = &VerBuff.Faces[k].Data[i]    [j];
                    radiosity_vertex_color * v1 = &VerBuff.Faces[k].Data[i]    [j + 1];
                    radiosity_vertex_color * v2 = &VerBuff.Faces[k].Data[i + 1][j];
                    radiosity_vertex_color * v3 = &VerBuff.Faces[k].Data[i + 1][j + 1];
                    Quads.push_back(quad(v0,v1,v2,v3));
                }
            }
        }
    }
};


#endif //RADIOSITY_VERTEX_H
