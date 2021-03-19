/* 
Radiosity Solver by Alejandro Armenta
3/16/2021
 YOUAREHERE: 23.5.2.1 Reference Deduction 
*/

#include "matrix.h"
#include "ortho_projector.h"


#if defined(_MSC_VER)
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
//#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
// Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
// allocations to be of _CLIENT_BLOCK type
#endif

int main()
{
#if defined(_MSC_VER)
    _CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	//_CrtSetBreakAlloc(147);	
#endif
    //OrthoProjector o{};
    //o.render();
    Matrix<double,2> m{
        { 0, 1, 2, 3},
        {10,11,12,13},
        {20,21,22,23},
    };
}