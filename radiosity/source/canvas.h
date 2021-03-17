/* date = March 16th 2021 3:30 pm */

#ifndef CANVAS_H
#define CANVAS_H

#include <string>
#include "vec3.h"

class Canvas{
    public:
    void set_base();
    bool open_file(std::string);
    bool not_finished_writing();
    float conv_text_to_uv(int);
    Vec3<float> conv_uv_to_world(float);
    Vec3<float> get_p_sample(Vec3<float>, Vec3<float>);
    void write_color(Color<float>&);
    private:
};

#endif //CANVAS_H
