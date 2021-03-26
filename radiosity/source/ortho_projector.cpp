#include "ortho_projector.h"

/* 
NO, we are not gonna request vertices to space interface, that doesn´t make sense
instead i have to put them wherever i want!!!
 */
void OrthoProjector::render(float fw, float fh, int th, int tw)
{
    float cg = 0.4f;
    
    // NOTE(Alex): XY (0,0) (5,5) Z = 0
    {
        Vec3<float> O{0,0,0};
        Vec3<float> X{1,0,0};
        Vec3<float> Y{0,1,0};
        Vec3<float> d{0,0,-1};
        Canvas canvas(X, Y, O, fw, fh, th, tw, cg);
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
    
    {
        /* 
        X has to be based from 0,0
Y as well
Origin has to be the origin!!!
         */
        // NOTE(Alex): YZ (0,0) (5,5) X = 0
        
        Vec3<float> O{0,0,fw};
        Vec3<float> X{0,0,-1};
        Vec3<float> Y{0,1,0};
        Vec3<float> d{-1,0,0};
        Canvas canvas(X, Y, O, fw, fh, th, tw, cg);
        if(canvas.open_ppm_file("YZ_X0.ppm"))
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
    
    
    {
        /* 
        X has to be based from 0,0
Y as well
Origin has to be the origin!!!
         */
        // NOTE(Alex): XZ (0,0) (5,5) Y = 0
        Vec3<float> O{fw,0,fw};
        Vec3<float> X{0,0,-1};
        Vec3<float> Y{-1,0,0};
        Vec3<float> d{0,-1,0};
        Canvas canvas(X, Y, O, fw, fh, th, tw, cg);
        if(canvas.open_ppm_file("XZ_Y0.ppm"))
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
    
    {
        /* 
        X has to be based from 0,0
Y as well
Origin has to be the origin!!!
         */
        // NOTE(Alex): YZ (0,0) (5,5) X = 5.0f
        Vec3<float> O{fw,0,0};
        Vec3<float> X{0,0,1};
        Vec3<float> Y{0,1,0};
        Vec3<float> d{1,0,0};
        Canvas canvas(X, Y, O, fw, fh, th, tw, cg);
        if(canvas.open_ppm_file("YZ_X5.ppm"))
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
    
    
    {
        /* 
        X has to be based from 0,0
Y as well
Origin has to be the origin!!!
         */
        // NOTE(Alex): XZ (0,0) (5,5) Y = 5.0f
        Vec3<float> O{fw,fw,0};
        Vec3<float> X{0,0,1};
        Vec3<float> Y{-1,0,0};
        Vec3<float> d{0,1,0};
        Canvas canvas(X, Y, O, fw, fh, th, tw, cg);
        if(canvas.open_ppm_file("XZ_Y5.ppm"))
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
}
