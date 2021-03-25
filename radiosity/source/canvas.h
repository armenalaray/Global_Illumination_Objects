/* date = March 16th 2021 3:30 pm */

#ifndef CANVAS_H
#define CANVAS_H

#include <utility>
#include <fstream>
#include <ios>
#include <string>
#include "vec3.h"

class Canvas{
    public:
    Canvas(const Vec3<float>& X_, const Vec3<float>& Y_, const Vec3<float>& O_, const float fw, const float fh, const int th, const int tw, const float cg_ = 0.0f);
    void set_base();
    void generate_ray();
    bool open_ppm_file(std::string);
    bool not_finished_writing();
    std::pair<float,float> conv_text_to_uv(int u, int v);
    std::pair<Vec3<float>,Vec3<float>> conv_uv_to_world(std::pair<float,float>);
    Vec3<float> get_p_sample();
    void write_color(Color<int>&);
    void close_ppm_file();
    private:
    Vec3<float> X;
    Vec3<float> Y;
    Vec3<float> Origin;
    float cg;
    Vec3<float> ShiftX;
    Vec3<float> ShiftY;
    int tx;
    int ty;
    float fw;
    float fh;
    int px;
    int py;
    std::ofstream fs;
};

#endif //CANVAS_H
