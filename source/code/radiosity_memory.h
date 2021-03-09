/* date = February 18th 2021 10:38 am */

#ifndef RADIOSITY_MEMORY_H
#define RADIOSITY_MEMORY_H


struct memory_arena
{
    void * Base;
    size_t Size;
    size_t Count;
    
    memory_arena()
    {
        Base = 0;
        Size = 0;
        Count = 0;
    }
};

#endif //RADIOSITY_MEMORY_H
