#include "persp_projector.h"

void PerspectiveProjector::render(float rfw, float rfh, int th, int tw){
    
#if 0    
    float fw=rfw + 0.0f;
    float fh=rfh + 0.0f;
    float h_fw=fw*0.5f;
    float h_fh=fh*0.5f;
    float h_rfw=rfw*0.5f;
    float h_rfh=rfh*0.5f;
    Vec3<float> p{h_rfw,h_rfh,rfw+22.0f};
    Vec3<float> O{h_rfw-h_fw,h_rfh-h_fh,rfw+2.0f};
#else
    float fw=0.2f*rfw;
    float fh=0.2f*rfh;
    float h_fw=fw*0.5f;
    float h_fh=fh*0.5f;
    float h_rfw=rfw*0.5f;
    float h_rfh=rfh*0.5f;
    Vec3<float> p{h_rfw,h_rfh,rfw+12.0f};
    Vec3<float> O{h_rfw-h_fw,h_rfh-h_fh,rfw+10.0f};
#endif
    
    Vec3<float> X{1,0,0};
    Vec3<float> Y{0,1,0};
    Vec3<float> d{0,0,-1};
    Canvas canvas(X, Y, O, fw, fh, th, tw);
    if(canvas.open_ppm_file("Radiosity.ppm"))
    {
        do
        {
            Vec3<float> pc = canvas.get_p_sample();
            Ray r{p,pc-p};
            Color<int> c = space.request_color(r, 0.001f, FLT_MAX);
            canvas.write_color(c);
        }
        while(canvas.not_finished_writing());
        canvas.close_ppm_file();
    }
}