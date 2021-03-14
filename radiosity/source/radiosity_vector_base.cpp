#include "radiosity_vector_base.h"


template<typename T, typename A>
vector_base<T,A>::vector_base(vector_base&& a):
alloc{a}, 
elem{a.elem},
space{a.space},
last{a.last}
{
    a.elem = a.space = a.last = nullptr;
}

template<typename T, typename A>
vector_base<T,A>& vector_base<T,A>::operator=(vector_base&& a)
{
    swap(*this,a);
    return *this;
}
