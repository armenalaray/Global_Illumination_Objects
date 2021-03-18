#include "ortho_projector.h"

/* 
NO, we are not gonna request vertices to space interface, that doesn´t make sense
instead i have to put them wherever i want!!!
 */
void OrthoProjector::render()
{
#if 0
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
#endif
    int th = 128;
    int tw = 128;
    float cg = 0.4f;
    
    // NOTE(Alex): XY (0,0) (5,5) Z = 0
    
    Vec3<float> O{0,0,0};
    Vec3<float> X{1,0,0};
    Vec3<float> Y{0,1,0};
    Vec3<float> d{0,0,-1};
    Canvas canvas(X, Y, O, th, tw, cg);
    if(canvas.open_ppm_file("XY_Z0.ppm"))
    {
        do
        {
            Vec3<float> p = canvas.get_p_sample();
            Ray r{p-d,d};
            Color<int> c = space.request_color(r, 0.001f, FLT_MAX);
            canvas.write_color(c);
        }
        while(canvas.not_finished_writing());
        canvas.close_ppm_file();
    }
}

#if 0
{
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
        
        vec3 ShiftX = (CanvasGap * 0.5f) * X;
        vec3 ShiftY = (CanvasGap * 0.5f) * Y;
        
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
}
#endif
