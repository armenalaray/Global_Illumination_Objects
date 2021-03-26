#include "face.h"


/* 
 Mapping constructor
Description:
Constructs a Mapping Object that stores possible mappings for each Quad within Face.

Parameters: 
const Mapped_quad_ur& ur_: Mapped Quad Upper Right.
const Mapped_quad_u& u_: Mapped Quad Upper.
const Mapped_quad_ul& ul_: Mapped Quad Upper Left.
const Mapped_quad_r& r_: Mapped Quad Right.
const Mapped_quad_k& k_: Mapped Quad Center (this is the same Quad as referenced by itself).
const Mapped_quad_l& l_: Mapped Quad Left.
const Mapped_quad_br& br_: Mapped Quad Bottom Right.
const Mapped_quad_b& b_: Mapped Quad Bottom.
const Mapped_quad_bl& bl_: Mapped Quad Bottom Left.

Output: -
 */
Mapping::Mapping(const Mapped_quad_ur& ur_,
                 const Mapped_quad_u& u_,
                 const Mapped_quad_ul& ul_,
                 const Mapped_quad_r& r_,
                 const Mapped_quad_k& k_,
                 const Mapped_quad_l& l_,
                 const Mapped_quad_br& br_,
                 const Mapped_quad_b& b_,
                 const Mapped_quad_bl& bl_):
ur{ur_},
u{u_},
ul{ul_},
r{r_},
k{k_},
l{l_},
br{br_},
b{b_},
bl{bl_}
{}


/* 
 void Face::generate_mapping()
Description:
Generates the corresponding Mapping object for each Quad.

Parameters: 

Output: -
 */
void Face::generate_mapping(){
    
    for(auto i=0; i<f.get_extent(0);++i){
        for(auto j=0; j<f.get_extent(1);++j){
            
            Mapped_quad_ur ur{};
            Mapped_quad_u u{};
            Mapped_quad_ul ul{};
            Mapped_quad_r r{};
            Mapped_quad_k k{};
            Mapped_quad_l l{};
            Mapped_quad_br br{};
            Mapped_quad_b b{};
            Mapped_quad_bl bl{};
            
            if(i+1<f.get_extent(0))
            {
                if(j+1<f.get_extent(1))
                    ur = {f(i+1,j+1)};
                
                u = {f(i+1,j)};
                
                if(j-1>=0)
                    ul = {f(i+1,j-1)};
            }
            
            if(j+1<f.get_extent(1))
                r = {f(i,j+1)};
            
            k = {f(i,j)};
            
            if(j-1>=0)
                l = {f(i,j-1)};
            
            if(i-1>=0){
                if(j+1<f.get_extent(1))
                    br = {f(i-1,j+1)};
                
                b = {f(i-1,j)};
                
                if(j-1>=0)
                    bl = {f(i-1,j-1)};
            }
            
            cm.push_back(std::pair<std::shared_ptr<Quad>,Mapping>{f(i,j),Mapping
                             {
                                 ur,
                                 u,
                                 ul,
                                 r,
                                 k,
                                 l,
                                 br,
                                 b,
                                 bl
                             }});
        }
    }
}

/* 
Face_xy_z0 Constructor
Description:
Constructs a Face_xy_z0 based upon parameters.

Parameters: 
float fw: Face Width.
 int hps: Hitables Per Side.
 ElemIndex& ei: Element Index.
 std::vector<std::shared_ptr<Quad>>& qmv: Vector of Quad pointers.

Output: -
 */
Face_xy_z0::Face_xy_z0(float fw, int hps, ElemIndex& ei, std::vector<std::shared_ptr<Quad>>& qmv):
Face(ei,hps,hps)
{
    float s = fw / float(hps);
    float z = 0.f;
    float y0 = 0.f;
    float y1 = y0 + s;
    for (size_t i = 0; i < hps ; i += 1, y0 += s, y1 += s){
        float x0 = 0.f;
        float x1 = x0 + s; 
        for (size_t j = 0; j < hps; j += 1, x0 += s, x1 += s, ei += 1){
            Vec3<float> p{x0+0.5f*s,y0+0.5f*s,z};
            f(i,j) = std::make_shared<Quad_XY_Z0>(p, ei, x0,x1,y0,y1,z);
        }
    }
    generate_mapping();
    for(auto i:cm)qmv.push_back(i.first);
}


/* 
 Face_yz_x0 Constructor
Description:
Constructs a Face_yz_x0 based upon parameters.

Parameters: 
float fw: Face Width.
 int hps: Hitables Per Side.
 ElemIndex& ei: Element Index.
 std::vector<std::shared_ptr<Quad>>& qmv: Vector of Quad pointers.

Output: -
 */
Face_yz_x0::Face_yz_x0(float fw, int hps, ElemIndex& ei, std::vector<std::shared_ptr<Quad>>& qmv):
Face(ei,hps,hps)
{
    float s = fw / float(hps);
    float x = 0.0f;
    float y0 = 0;
    float y1 = y0 + s;
    for (size_t i = 0; i < hps ; i += 1, y0 += s, y1 += s){
        float z1 = fw; 
        float z0 = z1 - s; 
        for (size_t j = 0; j < hps; j += 1, z1 -= s, z0 -= s, ei += 1){
            Vec3<float> p{x, y0+0.5f*s,z0+0.5f*s};
            f(i,j) = std::make_shared<Quad_YZ_X0>(p, ei, y0,y1,z0,z1,x);
        }
    }
    generate_mapping();
    for(auto i:cm)qmv.push_back(i.first);
}


/* 
 Face_xz_y0 Constructor
Description:
Constructs a Face_xz_y0 based upon parameters.

Parameters: 
float fw: Face Width.
 int hps: Hitables Per Side.
 ElemIndex& ei: Element Index.
 std::vector<std::shared_ptr<Quad>>& qmv: Vector of Quad pointers.

Output: -
 */
Face_xz_y0::Face_xz_y0(float fw, int hps, ElemIndex& ei, std::vector<std::shared_ptr<Quad>>& qmv):
Face(ei,hps,hps)
{
    float s = fw / float(hps);
    float y = 0.0f;
    float x1 = fw; 
    float x0 = x1 - s; 
    for (size_t i = 0; i < hps; i += 1, x0 -= s, x1 -= s){
        float z1 = fw;
        float z0 = z1 - s;
        for (size_t j = 0; j < hps ; j += 1, z0 -= s, z1 -= s, ei += 1){
            Vec3<float> p{x0+0.5f*s, y,z0+0.5f*s};
            f(i,j) = std::make_shared<Quad_XZ_Y0>(p, ei, x0,x1,z0,z1,y);
        }
    }
    generate_mapping();
    for(auto i:cm)qmv.push_back(i.first);
}


/* 
 Face_yz_x5 Constructor
Description:
Constructs a Face_yz_x5 based upon parameters.

Parameters: 
float fw: Face Width.
 int hps: Hitables Per Side.
 ElemIndex& ei: Element Index.
 std::vector<std::shared_ptr<Quad>>& qmv: Vector of Quad pointers.

Output: -
 */
Face_yz_x5::Face_yz_x5(float fw, int hps, ElemIndex& ei, std::vector<std::shared_ptr<Quad>>& qmv):
Face(ei,hps,hps)
{
    float s = fw / float(hps);
    float x = fw;
    float y0 = 0;
    float y1 = y0 + s;
    for (size_t i = 0; i < hps ; i += 1, y0 += s, y1 += s){
        float z0 = 0; 
        float z1 = z0 + s; 
        for (size_t j = 0; j < hps; j += 1, z0 += s, z1 += s, ei += 1){
            Vec3<float> p{x, y0+0.5f*s,z0+0.5f*s};
            f(i,j) = std::make_shared<Quad_YZ_X5>(p, ei, y0,y1,z0,z1,x);
        }
    }
    generate_mapping();
    for(auto i:cm)qmv.push_back(i.first);
}


/* 
 Face_xz_y5 Constructor
Description:
Constructs a Face_xz_y5 based upon parameters.

Parameters: 
float fw: Face Width.
 int hps: Hitables Per Side.
 ElemIndex& ei: Element Index.
 std::vector<std::shared_ptr<Quad>>& qmv: Vector of Quad pointers.

Output: -
 */
Face_xz_y5::Face_xz_y5(float fw, int hps, ElemIndex& ei, std::vector<std::shared_ptr<Quad>>& qmv):
Face(ei,hps,hps)
{
    float s = fw / float(hps);
    float y = fw;
    float x0 = 0; 
    float x1 = x0 + s; 
    for (size_t i = 0; i < hps ; i += 1, x0 += s, x1 += s){
        float z0 = 0;
        float z1 = z0 + s;
        for (size_t j = 0; j < hps; j += 1, z0 += s, z1 += s, ei += 1){
            Vec3<float> p{x0+0.5f*s, y,z0+0.5f*s};
            f(i,j) = std::make_shared<Quad_XZ_Y5>(p, ei, x0,x1,z0,z1,y);
        }
    }
    generate_mapping();
    for(auto i:cm)qmv.push_back(i.first);
}

/* 
 Face_emissor Constructor
Description:
Constructs a Face_emissor based upon parameters.

Parameters: 
float fw: Face Width.
 int hps: Hitables Per Side.
 ElemIndex& ei: Element Index.
 std::vector<std::shared_ptr<Quad>>& qmv: Vector of Quad pointers.

Output: -
 */
Face_emissor::Face_emissor(float fw, int hps, ElemIndex& ei, std::vector<std::shared_ptr<Quad>>& qmv):
Face(ei,1,1)
{
    float half_d = fw * 0.25f;
    float x0 = fw * 0.5f - half_d;
    float x1 = fw * 0.5f + half_d;
    float z0 = fw * 0.5f - half_d;
    float z1 = fw * 0.5f + half_d;
    float y=fw - 0.1f;
    Vec3<float> p{x0+0.5f*(x1-x0), y,z0+0.5f*(z1-z0)};
    f(0,0) = std::make_shared<Quad_XZ_Y5>(p, ei, x0,x1,z0,z1,y);
    generate_mapping();
    for(auto i:cm)qmv.push_back(i.first);
}

/* 
void Face::debug_print(std::string fn)
Description:
Prints first Vertex Color of each Quad within Face into PPM file.

Parameters: 
std::string fn: FileName.

Output: -
 */
void Face::debug_print(std::string fn)
{
    /* 
    We will print it top to bottom!!!
     */
    std::ofstream ofs;
    ofs.open(fn, std::ios::trunc | std::ios::out);
    if(ofs.is_open())
    {
        /* 
        First goes width then height
         */
        ofs << "P3\n" << f.get_extent(1) << " " << f.get_extent(0) << "\n255\n";
        
        for(int i=f.get_extent(0)-1;i>=0;--i){
            for(int j=0;j<f.get_extent(1);++j){
                auto a=f(i,j);
                Color<float> c = a->c[2];
                int ir = int(255.99 * c.r);
                int ig = int(255.99 * c.g);
                int ib = int(255.99 * c.b);
                ofs << std::to_string(ir) << " " << std::to_string(ig) << " " << std::to_string(ib) << std::endl;
            }
        }
        ofs.close();
    }
    else std::cout << "Unable to open file:" << fn << std::endl;
}

/* 
void Face::add_radiosities
Description:
Adds radiosity results onto each Quad within Face. The reference starts with si as base.

Parameters: 
const Matrix<float,1>& r: Vector With radiosity results for red channel.
const Matrix<float,1>& g: Vector With radiosity results for green channel.
const Matrix<float,1>& b: Vector With radiosity results for blue channel.

Output: -
 */
void Face::add_radiosities(const Matrix<float,1>& r,const Matrix<float,1>& g,const Matrix<float,1>& b){
    ElemIndex ei=si;
    for(auto a:cm){
        auto mapping=a.second;
        mapping.ur.add_color(Color<float>{r(ei),g(ei),b(ei)});
        mapping.u.add_color(Color<float>{r(ei),g(ei),b(ei)});
        mapping.ul.add_color(Color<float>{r(ei),g(ei),b(ei)});
        mapping.r.add_color(Color<float>{r(ei),g(ei),b(ei)});
        mapping.k.add_color(Color<float>{r(ei),g(ei),b(ei)});
        mapping.l.add_color(Color<float>{r(ei),g(ei),b(ei)});
        mapping.br.add_color(Color<float>{r(ei),g(ei),b(ei)});
        mapping.b.add_color(Color<float>{r(ei),g(ei),b(ei)});
        mapping.bl.add_color(Color<float>{r(ei),g(ei),b(ei)});
        ++ei;
    }
    if(f.get_extent(0) > 1 && f.get_extent(1) > 1)
    {
        for(auto i=0;i<f.get_extent(0);++i){
            for(auto j=0;j<f.get_extent(1);++j){
                auto a = f(i,j);
                if(i==0 && j==0){
                    a->c[3] /= 4;
                    a->c[1] /= 2;
                    a->c[2] /= 2;
                }
                else if(i==0 && j==f.get_extent(1)-1){
                    a->c[2] /= 4;
                    a->c[3] /= 2;
                    a->c[0] /= 2;
                }
                else if(i==f.get_extent(0)-1 && j==0){
                    a->c[1] /= 4;
                    a->c[3] /= 2;
                    a->c[0] /= 2;
                }
                else if(i==f.get_extent(0)-1 && j==f.get_extent(1)-1)
                {
                    a->c[0] /= 4;
                    a->c[2] /= 2;
                    a->c[1] /= 2;
                }
                else if(i==0)
                {
                    a->c[2] /= 4;
                    a->c[3] /= 4;
                    a->c[0] /= 2;
                    a->c[1] /= 2;
                }
                else if(i==f.get_extent(0)-1)
                {
                    a->c[2] /= 2;
                    a->c[3] /= 2;
                    a->c[0] /= 4;
                    a->c[1] /= 4;
                }
                else if(j==0)
                {
                    a->c[3] /= 4;
                    a->c[1] /= 4;
                    a->c[2] /= 2;
                    a->c[0] /= 2;
                }
                else if(j==f.get_extent(1)-1)
                {
                    a->c[2] /= 4;
                    a->c[0] /= 4;
                    a->c[3] /= 2;
                    a->c[1] /= 2;
                }
                else
                {
                    a->c[0] /= 4;
                    a->c[1] /= 4;
                    a->c[2] /= 4;
                    a->c[3] /= 4;
                }
            }
        }
    }
}
