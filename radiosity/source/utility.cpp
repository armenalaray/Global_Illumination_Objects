
template<typename T = float> 
inline T Lerp(const T &lo, const T &hi, const float t) 
{ 
    return lo * (1 - t) + hi * t; 
}
