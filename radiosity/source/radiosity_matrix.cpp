#include "radiosity_matrix.h"

radiosity_matrix::radiosity_matrix(int Value_)
{
    Value = Value_;
}

#if 0
template<typename M>
Enable_if<Matrix_type<M>(), ostream&>
operator<<(ostream & os, const M& m)
{
    os << '{';
    for(size_t i = 0; i != rows(m); ++i)
    {
        os << m[i];
        if(i+1!=rows(m)) os << ',';
    }
    return os << '}';
}
#endif