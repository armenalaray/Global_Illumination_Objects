#include "hemi_cube.h"

HemiCube::HemiCube(int xc_, int yc_):
xc{xc_},
yc{yc_},
y_halfc{static_cast<int>(float(yc)*0.5f)},
tfs{2.0f},
dx{tfs/xc},
dy{tfs/yc},
half_pw{dx*0.5f},
half_ph{dy*0.5f},
da{dx*dy},
vup{0,1,0},
other_vup{1,0,0}
{
    
#if 0    
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
#endif
    
}

bool HemiCube::get_next_element(Element& e){
    return false;
}

