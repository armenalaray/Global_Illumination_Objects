#include "radiosity_matrix.h"

#define OPTIMIZED_ASIGMNT_OPERATOR
#define RESERVE_NOT_INITIALIZE


template<typename T, typename A>
void vector<T,A>::destroy_elements()
{
    for(T* p=vb.elem;p!=vb.space;++p)
        p->~T();
    vb.space=vb.elem;
}

template<typename T, typename A>
vector<T,A>::vector(size_type n, const T& val, const A& a):
vb{a,n}
{
    uninitialized_fill(vb.elem,vb.elem+n,val);
}

template<typename T, typename A>
vector<T,A>::vector(const vector<T,A>& a):
vb{a.vb.alloc,a.size()}
{
    //aqui se llama al destructor de miembros base para deallocar memoria
    uninitialized_copy(a.begin(),a.end(),vb.elem);
}

template<typename T, typename A>
vector<T,A>::vector(vector&& a):
vb{move(a.vb)}
{
}

#if defined(BASIC_ASIGMNT_OPERATOR)

// NOTE(Alex): this has to keep the strong guarantee!!
template<typename T,typename A>
vector<T,A>& vector<T,A>::operator=(const vector& a)
{
    // NOTE(Alex): esto mantiene la integridad de vb, hasta que este seguro de que hay suficiente memoria 
    //para alocar, destruyo vb y copio elementos a vb desde b
    vector_base<T,A> b{a.vb.alloc, a.size()}; //get memory
    uninitialized_copy(a.begin(),a.elem(),b.elem); //copy elements onto b
    destroy_elements();
    swap(vb,b);
    return *this;
}

#elif defined(SIMPLIFIED_ASIGMNT_OPERATOR)

// NOTE(Alex): this has to keep the strong guarantee!!
template<typename T,typename A>
vector<T,A>& vector<T,A>::operator=(const vector& a)
{
    // NOTE(Alex): porque yo ya se que esto funciona, no necesito preocuparme por asegurarme de que funciona o no
    //esto siempre va a tratar de crear una copia, eso es todo, ya con seguridad de excepcion y offser the strong guarnateee
    vector temp{a};
    
    swap(*this,temp);
    /* 
    Aqui temp destruye los elementos de *this entonces no hay necesidad de delete
Aha osea tienes que estar seguro de que tus bases funcionan para asi estar seguro de que lo que haces es correcto. simplemente reutilizas tus bases 
siempre que puedas constructores base, clases base
clases base miembro de soporte de memoria!!!
     */
    return *this;
}

#elif defined(OPTIMIZED_ASIGMNT_OPERATOR)
/* 
If the capacity of the vector assigned to is large enough to hold the assigned vector, we don´t need}
to allocate new memory.

 */
// NOTE(Alex): this has to keep the basic guarantee
template<typename T,typename A>
vector<T,A>& vector<T,A>::operator=(const vector& a)
{
    if(capacity() < a.size()){
        vector temp{a};
        swap(*this,temp);
        return *this;
    }
    
    if(this == &a)return *this;
    
    size_type sz = size();
    size_type asz = a.size();
    vb.alloc = a.vb.alloc;
    
    if(asz <= sz){
        copy(a.begin(),a.begin()+asz,vb.elem);
        for(T* p=vb.elem+asz;p!=vb.space; ++p)
            p->~T();                            //destroy surplus elements
    }
    else
    {
        //este es para el caso que hay capacidad pero el size no es suficiente
        copy(a.begin(),a.begin()+sz,vb.elem);   //solo copia los que caben
        uninitialized_copy(a.begin()+sz,a.end(),vb.space); //construct extra elements
    }
    
    vb.space = vb.elem+asz;
    return *this;
}

#endif

template<typename T, typename A>
vector<T,A>& vector<T,A>::operator=(vector&& a)
{
    clear();
    swap(vb,a.vb);
    return *this;
}

#if defined(RESERVE_INITIALIZE)

template<typename T, typename A>
void vector<T,A>::reserve(size_type newalloc){
    if(newalloc<=capacity())return;
    vector<T,A> b{newalloc}; //OK: explicit
    copy(vb.elem,vb.elem+size(),b.begin());
    vb.space=size(); // NOTE(Alex): this is for making destructor in v destroy all the old elements in this
    swap(vb,b);
}//implicitly release old space

#elif defined(RESERVE_NOT_INITIALIZE)

/* 
We don´t want the extra element slots initialized.
and we don´t want to iterate twice first to initialize and then to copy
 */
template<typename T, typename A>
void vector<T,A>::reserve(size_type newalloc){
    if(newalloc<=capacity())return;
    vector_base<T,A> b{vb.alloc,size(),newalloc-size()};
    uninitialized_move(vb.elem, vb.elem+size(),b.elem);
    swap(vb,b);
}

/* 
A move operation should not throw
 */
template<typename In, typename Out>
Out uninitialized_move(In b, In e, Out oo)
{
    using T=Value_type<Out>;
    for(;b!=e;++b,++oo){
        // NOTE(Alex): if you don´t use move, you don´t move you copy
        new(static_cast<void*>(&*oo))T{move{*b}}; //move construct
        b->~T();
    }
    return oo;
}

#endif

template<typename T, typename A>
void vector<T,A>::resize(size_type newsize, const T& val)
{
    reserve(newsize);
    if(size()<newsize)
        uninitialized_fill(vb.elem+size(),vb.elem+newsize,val);
    else
        destroy(vb.elem+newsize, vb.elem+size());
    vb.space = vb.elem+newsize;
}

template<typename In>
void destroy(In b, In e)
{
    for(;b!=e;++b) //destroy [b:e)
        b->~Value_type<In>();
}


template<class T, typename A>
void vector<T,A>::push_back(const T& vañ)
{
    if(capacity()==size())
        reserve(size()?2*size():8); //grow or start with 8
    vb.alloc.construct(&vb.elem[size()],val);
    ++vb.space;
}

/*
This templatized version is cleaner and shorter because it can rely on more information about the type of the elements it sorts.
Typically, it is also faster because it doesn´t rely on a pointer to function for the comparison. this
implies that no indirect function calls are needed and that inlining a simple < is easy.
*/

template<typename T>
void sort(vector<T>& v)
{
    const size_t n=v.size();
    for(int gap=n/2;0<gap;gap/=2){
        for(int i=gap;i<n;i++){
            for(int j=i-gap;0<=j;j-=gap){
                // NOTE(Alex): operator compare < for elements in vector
                if(v[j+gap]<v[j]){ //swap v[j] and v[j+gap]
                    // NOTE(Alex): operator asignment for elements in vector
                    T temp=v[j];
                    v[j] = v[j+gap];
                    v[j+gap] = temp;
                }
            }
        }
    }
}
//std:less<T> - Unless specialized, invokes operator< on type T.
template<typename T, typename Compare = std:less<T>>
void sort(vector<T>& v)
{
    // NOTE(Alex): ok entonces se hacen objetos asi genericos. compare
    Compare cmp;   //Make a default compare object.
    const size_t n = v.size();
    for(int gap=n/2;0<gap;gap/=2){
        for(int i=gap;i<n;i++){
            for(int j=i-gap;0<=j;j-=gap){
                if(cmp(v[j+gap],v[j]))
                    swap(v[j],v[j+gap]);
            }
        }
    }
}

// we can  now sort using the default comparison operationn (<) or supply our own:
struct No_case{
    bool operator()(const string& a, const string& b)const; //compare case insensitive
};


/* 
Asi es como se sobrecarga un operador de comparación!!!

 */
void matrix_f(vector<int>& vi, vector<string>& vs)
{
    sort(vi);                         //sort(vector<int>&)
    sort<int,std::greater<int>>(vi);  //sort(vector<int>&) using greater
    
    sort(vs);                         //sort(vector<string>&)
    sort<string,No_case>(vs);         //sort(vector<string>&) using No_case
}
