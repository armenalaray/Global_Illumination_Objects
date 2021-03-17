/* date = March 13th 2021 4:31 pm */

#ifndef RADIOSITY_VECTOR_BASE_H
#define RADIOSITY_VECTOR_BASE_H

// NOTE(Alex): if uninitialized_copy fails destructor of base classes are called and memory deallocated for vector_base
/* 
por eso es que se diseño como por miembros o por herencia, es lo mismo, el destructor se llama antas de llamar al destructor derivada
ese es el punto de utilizar clases, reutilizar codigo!!!
por eso es importante que vea eso, elementos base, abstractos, etc!!
e implementarlos!
 */

template<typename T, typename A = allocator<T>>
struct vector_base{
    A alloc; 
    T* elem;
    T* space;
    T* last;
    
    using size_type = typename A::size_type;
    
    vector_base(const A& a, size_type n, size_type m=0):
    alloc{a},elem{alloc.allocate(n+m)},space{elem+n},last{elem+n+m}{}
    ~vector_base(){alloc.deallocate(elem,last-elem);}
    vector_base(const vector_base&)=delete;
    vector_base& operator=(const vector_base&)=delete;
    vector_base(vector_base&&);
    vector_base& operator=(vector_base&&);
};


#endif //RADIOSITY_VECTOR_BASE_H
