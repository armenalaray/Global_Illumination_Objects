#include "hemi_cube.h"

/* 
Basic Description of the Algorithm: NOTE(for a thorough explanation see README).
-The Hemicube is an algorithm to compute object to object diffuse 
 -interreflections within complex environments.

-The phenomena of Color Bleeding is accurately reproduced. 
-Penumbras along shadow boundaries.
-And shading within envelopes.

HemiCube Constructor
Description:
Contructs an HemiCube centered in Element node.

Parameters: 
int xc_: Cube's Pixel Count per Face width.
int yc_: Cube's Pixel Count per Face height.

Members:
xc: Cube's Pixel Count per Face width.
yc: Cube's Pixel Count per Face height.
y_halfc: Half Pixel Height per Face.
tfs: Hemicube's Face Length. 
dx: Pixel Width.
dy: Pixel Height.
half_pw: Half Pixel Width.
half_ph: Half Pixel Height.
da: Pixel Area.
vup: Vector representing up, to construct coordinate system.
other_vup: Other Vector representing up, to construct coordinate system.
x: PixelIndex X.
y: PixelIndex Y.

Output: -
 */
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

