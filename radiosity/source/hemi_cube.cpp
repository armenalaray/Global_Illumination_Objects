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
other_vup{1,0,0},
x{0},
y{0}
{
    
}

