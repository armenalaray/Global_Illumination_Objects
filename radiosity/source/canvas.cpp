#include "canvas.h"

void Canvas::set_base()
{
    
}

bool Canvas::open_file(std::string file_s)
{
    return false;
}

bool Canvas::not_finished_writing()
{
    return false;
}

float Canvas::conv_text_to_uv(int v)
{
    return 0.f;
}

Vec3<float> Canvas::conv_uv_to_world(float v)
{
    return {0,0,0};
}

Vec3<float> get_p_sample(Vec3<float> px, Vec3<float> py)
{
    return {0,0,0};
}

void Canvas::write_color(Color<float>& c)
{
    
}