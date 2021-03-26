#include "mapped_quad.h"
/*

Vertex Colors Map reference:

 c2  -------  c3
-            -
-            -
-            -
-            -
c0  -------  c1
 
*/

/* 
void Mapped_quad_ur::add_color(const Color<float>& c)
Description:
Adds color to Vertex Color c0.

Parameters: 
const Color<float>& c: Color floating point values.

Output: -
 */
void Mapped_quad_ur::add_color(const Color<float>& c){
    if(qp.get())
    {
        qp->c[0]+=c;
    }
}

/* 
void Mapped_quad_ur::add_color(const Color<float>& c)
Description:
Adds color to Vertex Color c0 and c1.

Parameters: 
const Color<float>& c: Color floating point values.

Output: -
 */
void Mapped_quad_u::add_color(const Color<float>& c){
    if(qp.get())
    {
        qp->c[0]+=c;
        qp->c[1]+=c;
    }
}

/* 
void Mapped_quad_ur::add_color(const Color<float>& c)
Description:
Adds color to Vertex Color c1.

Parameters: 
const Color<float>& c: Color floating point values.

Output: -
 */
void Mapped_quad_ul::add_color(const Color<float>& c){
    if(qp.get())
    {
        qp->c[1]+=c;
    }
}

/* 
void Mapped_quad_ur::add_color(const Color<float>& c)
Description:
Adds color to Vertex Color c0 and c2.

Parameters: 
const Color<float>& c: Color floating point values.

Output: -
 */
void Mapped_quad_r::add_color(const Color<float>& c){
    if(qp.get())
    {
        qp->c[0]+=c;
        qp->c[2]+=c;
    }
}

/* 
void Mapped_quad_ur::add_color(const Color<float>& c)
Description:
Adds color to Vertex Color c0 c1 c3 and c3.
Parameters: 
const Color<float>& c: Color floating point values.

Output: -
 */
void Mapped_quad_k::add_color(const Color<float>& c){
    if(qp.get())
    {
        qp->c[0]+=c;
        qp->c[1]+=c;
        qp->c[2]+=c;
        qp->c[3]+=c;
    }
}

/* 
void Mapped_quad_ur::add_color(const Color<float>& c)
Description:
Adds color to Vertex Color c1 and c3

Parameters: 
const Color<float>& c: Color floating point values.

Output: -
 */
void Mapped_quad_l::add_color(const Color<float>& c){
    if(qp.get())
    {
        qp->c[1]+=c;
        qp->c[3]+=c;
    }
}

/* 
void Mapped_quad_ur::add_color(const Color<float>& c)
Description:
Adds color to Vertex Color c2
.
Parameters: 
const Color<float>& c: Color floating point values.

Output: -
 */
void Mapped_quad_br::add_color(const Color<float>& c){
    if(qp.get())
    {
        qp->c[2]+=c;
    }
}

/* 
void Mapped_quad_ur::add_color(const Color<float>& c)
Description:
Adds color to Vertex Color c2 and c3.
Parameters: 
const Color<float>& c: Color floating point values.

Output: -
 */
void Mapped_quad_b::add_color(const Color<float>& c){
    if(qp.get())
    {
        qp->c[2]+=c;
        qp->c[3]+=c;
    }
}

/* 
void Mapped_quad_ur::add_color(const Color<float>& c)
Description:
Adds color to Vertex Color c3.
Parameters: 
const Color<float>& c: Color floating point values.

Output: -
 */
void Mapped_quad_bl::add_color(const Color<float>& c){
    if(qp.get())
    {
        qp->c[3]+=c;
    }
}
