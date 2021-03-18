/* date = March 16th 2021 7:19 pm */

#ifndef UTILITY_H
#define UTILITY_H

template<typename T = float> 
inline T Lerp(const T &lo, const T &hi, const float t) 
{ 
    return lo * (1 - t) + hi * t; 
}

#endif //UTILITY_H
