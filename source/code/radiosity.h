/* date = February 17th 2021 7:46 pm */

#ifndef RADIOSITY_H
#define RADIOSITY_H

#include <vector>
#include <iostream>
#include <iomanip>
//#include "stdio.h"
#include "stdint.h"
//#include "stdlib.h"


using namespace std;

#define KILOBYTES(i) (1024 * i)
#define MEGABYTES(i) (1024 * KILOBYTES(i))
#define GIGABYTES(i) (1024 * MEGABYTES(i))

#define Assert(Statement) if((Statement) == false) { *(int *)0 = 0; } 

class radiosity_state
{
    public:
    
    void * Memory;
    size_t BytesUsed;
    size_t ByteAllocSize;
    
    memory_arena MatrixArena;
    memory_arena TempMatrixArena;
    
    radiosity_state(size_t ByteAllocSize_) : MatrixArena(), TempMatrixArena()
    {
        Memory = malloc(ByteAllocSize_);
        if(Memory != NULL)
        {
            BytesUsed = 0;
            ByteAllocSize = ByteAllocSize_;
            MakeMemoryArena(&MatrixArena, MEGABYTES(1));
            MakeMemoryArena(&TempMatrixArena, MEGABYTES(1));
        }
        else
        {
            BytesUsed = 0;
            ByteAllocSize = 0;
            Memory = 0;
        }
    }
    
    void MakeMemoryArena(memory_arena * Arena, size_t ByteCount)
    {
        Assert((BytesUsed + ByteCount) <= ByteAllocSize);
        
        Arena->Size = ByteCount;
        Arena->Base = (uint8_t*)Memory + BytesUsed;
        Arena->Count = 0;
        
        BytesUsed += ByteCount;
    }
};

#endif //RADIOSITY_H
