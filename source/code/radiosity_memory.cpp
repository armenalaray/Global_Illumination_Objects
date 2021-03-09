/*
radiosity_memory.cpp
Manages memory for radiosity program
*/


#if 0
void * PushMemoryArena(memory_arena * Arena, size_t Count)
{
    void * Result = 0;
    Assert((Arena->Count + Count) <= Arena->Size);
    Result = (uint8_t*)Arena->Base + Arena->Count;
    Arena->Count += Count;
    return Result;
}

void FreeMemoryArena(memory_arena Arena)
{
    Arena.Count = 0;
}

template<typename T>
void PushVector(memory_arena * Arena, size_t n, matrix<T> * M)
{
    M->RowCount = n;
    M->ColumnCount = 1;
    M->Data = (T*)PushMemoryArena(Arena, sizeof(T) * n);
}

template<typename T>
void PushMatrix(memory_arena * Arena, size_t n, size_t m, matrix<T> * M)
{
    M->RowCount = n;
    M->ColumnCount = m;
    M->Data = (T*)PushMemoryArena(Arena, sizeof(T) * n * m);
}

template<typename T>
void PushDiagonalMatrix(memory_arena * Arena, size_t n, size_t m, matrix<T> * M, matrix<T> * Initializer = 0)
{
    PushMatrix(Arena, n, m, M);
    if(Initializer)
    {
        for (size_t i = 0; i < n ; i += 1){
            (*M)[i][i] = (*Initializer)[i][0];
        }
    }
}

template<typename T>
void PushIdentityMatrix(memory_arena * Arena, size_t n, size_t m, matrix<T> * M)
{
    PushMatrix(Arena, n, m, M);
    for (size_t i = 0; i < n ; i += 1){
        (*M)[i][i] = (T)1.0f;
    }
}
#endif
