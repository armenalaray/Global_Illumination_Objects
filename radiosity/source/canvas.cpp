#include "canvas.h"

/* 
Canvas constructor
Description:
Constructs a Canvas object, operations to write to PPM file.

Parameters: 
const Vec3<float>& X_: Width Axis.
const Vec3<float>& Y_: Height Axis.
const Vec3<float>& O_: Canvas position in World-Space.
const float fw_: Canvas Face Width in (World-Space size).
 const float fh_: Canvas Face Height in (World-Space size).
 const int th: Output image pixel height.
 const int tw: Output image pixel width.
 const float cg_: Padding in canvas, it forms a uniform padding around canvas, this is to protect ortho projectors from casting rays onto elements parallel to ray.

Output: -

 */
Canvas::Canvas(const Vec3<float>& X_, const Vec3<float>& Y_, const Vec3<float>& O_, const float fw_, const float fh_, const int th, const int tw, const float cg_): 
X{X_}, 
Y{Y_},
Origin{O_},
cg{cg_}, 
ShiftX{cg * 0.5f * X},
ShiftY{cg * 0.5f * Y},
tx{tw}, 
ty{th}, 
fw{fw_},
fh{fh_},
px{0}, 
py{th - 1}
{}


/* 
Canvas::open_ppm_file
Description:
Opends a PPM file and stores handle into fs.

Parameters: 
std::string file_s: FileName.

Output:
bool: Whether fs gets File handle successfully.
 */
bool Canvas::open_ppm_file(std::string file_s)
{
    fs.open(file_s, std::ios::trunc | std::ios::out);
    if(fs.is_open()){
        fs << "P3\n" << tx << " " << ty << "\n255\n";
        return true;
    }
    return false;
}

/* 
bool Canvas::not_finished_writing
Description:
Checks for writing position and ends writing operation.

Parameters: 

Output:
bool: Whether writing operation has ended.
 */
bool Canvas::not_finished_writing()
{
    if(py < 0) return false;
    if(px >= tx) {px = 0; --py;}
    ++px;
    return true;
}

/* 
Vec3<float> Canvas::get_p_sample()
Description:
Samples canvas and converts an image space sample into a World Space sample, based upon parameters given in constructor.

Parameters: 

Output:
Vec3<float> A world space sample.
 */
Vec3<float> Canvas::get_p_sample()
{
    auto uv = conv_text_to_uv(px,py);
    auto pxy = conv_uv_to_world(uv);
    return std::get<0>(pxy) + std::get<1>(pxy) + Origin + ShiftX + ShiftY;
}

/* 
std::pair<float,float> Canvas::conv_text_to_uv(int u, int v)
Description:
Converts Texture Coordinates to UV coordinates.

Parameters: 
int u: X texture Coordinate.
int v: Y texture Coordinate.

Output:
std::pair<float,float>: a pair Object that returns UV coordinates.
 */
std::pair<float,float> Canvas::conv_text_to_uv(int u, int v)
{
    return 
    {
        (static_cast<float>(u) / static_cast<float>(tx)), 
        (static_cast<float>(v) / static_cast<float>(ty)), 
    };
}

/* 
std::pair<Vec3<float>,Vec3<float>> Canvas::conv_uv_to_world(std::pair<float,float> p)
Description:
Converts UV coordinates to World Coordinates based on axis given in constructor.

Parameters: 
std::pair<float,float> p: a pair Object with uv coordinates.

Output:
std::pair<Vec3<float>,Vec3<float>> a pair Object with World Coordinates.
 */
std::pair<Vec3<float>,Vec3<float>> Canvas::conv_uv_to_world(std::pair<float,float> p)
{
    return 
    {
        (std::get<0>(p) * (fw - cg)) * X,
        (std::get<1>(p) * (fh - cg)) * Y,
    };
}

/* 
void Canvas::write_color(Color<int>& c)
Description:
Writes color into file.

Parameters: 
Color<int>& c Tri-stimulus integer based color. Range [0-255]

Output: -
 */
void Canvas::write_color(Color<int>& c)
{
    fs << c.r << " " << c.g << " " << c.b << std::endl;
}

/* 
void Canvas::close_ppm_file()
Description:
Closes ppm file.

Parameters: 

Output: -
 */
void Canvas::close_ppm_file(){
    fs.close();
}