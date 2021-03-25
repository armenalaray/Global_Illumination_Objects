/* 
Radiosity Solver by Alejandro Armenta
3/16/2021
 YOUAREHERE: 23.5.2.1 Reference Deduction 
*/

#include "space.h"
#include "ortho_projector.h"

#if defined(_MSC_VER)
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
//#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
// Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
// allocations to be of _CLIENT_BLOCK type
#endif

float fw=5.0f;
int hps=5;

Space space{fw,hps};

/* 
La interfaz es el render, o los renders no el radiosity!!!
 */

int main()
{
#if defined(_MSC_VER)
    _CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	//_CrtSetBreakAlloc(147);	
#endif
    
#if 0    
    Matrix<float,2> a(2,2);
    a(0,0)=1;
    a(0,1)=2;
    a(1,0)=3;
    a(1,1)=4;
    std::cout << a << std::endl;
    
    Matrix<float,2> b(2,2);
    b(0,0)=2;
    b(0,1)=0;
    b(1,0)=0;
    b(1,1)=1;
    std::cout << b << std::endl;
    
    Matrix<float,2> c(2,2);
    //c=sub_m(b,a);
    //std::cout << c << std::endl;
    c=mult_m(a,b);
    std::cout << c << std::endl;
    
    Matrix<float,2> a(3,3);
    a(0,0)=0;
    a(0,1)=1;
    a(0,2)=0;
    a(1,0)=1;
    a(1,1)=0;
    a(1,2)=0;
    a(2,0)=0;
    a(2,1)=0;
    a(2,2)=1;
    std::cout << a << std::endl;
    
    Matrix<float,2> b(3,3);
    b(0,0)=1;
    b(0,1)=2;
    b(0,2)=3;
    b(1,0)=4;
    b(1,1)=5;
    b(1,2)=6;
    b(2,0)=7;
    b(2,1)=8;
    b(2,2)=9;
    std::cout << b << std::endl;
    
    Matrix<float,2> c(3,3);
    c=mult_m(a,b);
    std::cout << c << std::endl;
#endif
    OrthoProjector o{};
    o.render(fw,fw,128,128);
}