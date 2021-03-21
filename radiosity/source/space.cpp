#include "space.h"

Color<int> Space::request_color(Ray r, float tMin, float tMax)
{
    HitRec rec{};
    int Index{-1};
    for(int i = 0; i < quads.size(); ++i)
    {
        if(quads[i]->hit(r, tMin, tMax, rec))
        {
            tMax = rec.t;
            Index = i;
        }
    }
    
    if(Index>=0) return quads[Index]->get_color(rec.u,rec.v);
    else return {0,0,0};
}

Element Space::request_element(Ray r, float tMin, float tMax)
{
    Element res{};
    return res;
}

