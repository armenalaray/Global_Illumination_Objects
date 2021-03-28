/* 
Radiosity Solver by Alejandro Armenta
3/16/2021
 YOUAREHERE: 23.5.2.1 Reference Deduction 
*/

#include "space.h"
#include "ortho_projector.h"
#include "persp_projector.h"

#if defined(_MSC_VER)
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
//#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
// Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
// allocations to be of _CLIENT_BLOCK type
#endif

float fw=10.0f;
int hps=10;
Space space{fw,hps};

int main()
{
#if defined(_MSC_VER)
    _CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	//_CrtSetBreakAlloc(147);	
#endif
    
    OrthoProjector o{};
    o.render(fw,fw,1024,1024);
    PerspectiveProjector p{};
    p.render(fw,fw,1024,1024);
}