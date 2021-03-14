#include "radiosity_matrix.h"

#define OPTIMIZED_ASIGMNT_OPERATOR
#define RESERVE_NOT_INITIALIZE

#if 0
template<typename T>
Matrix::Matrix(const Matrix& m) :
dim{m.dim},
elem{static_cast<T*>(new char[sizeof(T)*m.size()])}
{
    uninitialized_copy(m.elem,m.elem+m.size(),elem);
}
#endif

#if 0
template<typename T, typename A>
vector_base<T,A>::vector_base(vector_base&& a):
alloc{a.alloc},
elem{a.elem},
space{a.space},
last{a.last}
{
    a.elem = a.space = a.last = nullptr; //no longer owns any memory
}


/* 
 el alocador no se copia, because it belongs to vector!!! is not a member!!!
el alocador no se copia, pero se puede mover!!!
*/

template<typename T, typename A>
vector_base<T,A>& vector_base<T,A>::operator=(vector_base&& a)
{
    // NOTE(Alex): There are no objects of type T to destroy: vector_base deals with memory and leaves convers about objects of type T to vector.
    swap(*this,a);
    return *this;
}

template<typename T, typename A>
void vector<T,A>::destroy_elements()
{
    for(T* p=vb.elem; p!=vb.space;++p)
    {
        p->~T();                          //destroy element;
    }
    vb.space=vb.elem;
}

template<typename T,typename A>
vector<T,A>::vector(size_type n, const T& val, const A& a):
vb{a,n}
{
    uninitialized_fill(vb.elem,vb.elem+n,val); //make n copies of val
}

template<typename T, typename A>
vector<T,A>::vector(const vector<T,A>& a):
// NOTE(Alex): no vas a copiar el alocador 
//vas a iniciar el alocador con el alocador y el size
//inicializas el vector_base no lo copias!!! ya que no puedes copiarlo!!!

/* 
 Fundamental language rule that when an exception is thrown from a 
constructor, subobjects(including bases) that have already been completely constructed will be
properly destroyed.

The unitialized fill() algorithm and its cousins (13.6.1) provide
the equivalent quarantee for partially constructed sequences.
*/

vb{a.vb.alloc,a.size()}
{
    uninitialized_copy(a.begin(),a.end(),vb.elem);
}

#endif

#if 0
template<typename For, typename T>
void uninitialized_fill(For bed,For end, const T& x)
{
    For p;
    try{
        for(p=beg;p!=end;++p)
            ::new(static_cast<void*>(&*p))T(x); //construct copy of x in *p
    }
    catch(...){
        for(For q = beg;q!=p;++q)
            (&*q)->~T();                            //destroy element
        throw;
    }
}
#endif

/* 
There are two potential sources of exceptions here:

//allocate may throw an exception if no memory is available.
//T´s copy constructor may throw an exception if it can't copy val.

 */

#if 0
template<typename T, typename A>
vector<T,A>::vector(size_type n, const T& val, const A& a): //warning naive implementation
alloc{a}
{
    elem = alloc.allocate(n);       //get memory for elements
    space = last = elem + n;
    for(T*p=elem;p!=last;++p)
        a.construct(p,val);         //construct copy of val in *p
}
#endif

#if 0
template<typename T, typename A>
vector<T,A>::vector<size_type n, const T& val, const A& a): //elaborate implementaton
alloc{a} //copy the allocator
{
    
    elem=alloc.allocate(n);         //get memory for elements
    
    // TODO(Alex): handle the empty vector case if(n)
    
    T*p; //outside try{} because we need it in catch{}
    try{
        auto end = elem+n;
        for(p=elem; p!=end;++p)
            alloc.construct(p,val);  //construct copy of element
        last=space=p;
    }
    catch(...){
        for(auto q=element; q!=p++q)
            alloc.destroy(q);        //destroy constructed elements
        alloc.deallocate(elem,n);    //free memory
        throw;                       //rethrow
    }
}
#endif

#if 0
template<typename T, typename A>
vector<T,A>::vector(size_type n, const T& val, const A& a): //elaborate implementaton
alloc{a} //copy the allocator
{
    elem=alloc.allocate(n);         //get memory for elements
    
    // TODO(Alex): handle the empty vector case if(n)
    
    try{
        uninitialized_fill(elem,elem+n,val); //copy elements
        space=last=elem+n;
    }
    catch(...){
        alloc.deallocate(elem,n);    //free memory
        throw;                       //rethrow
    }
}
#endif


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
    vector<T,A> b(newalloc);
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
    uninitialized_move(vb.elem, vb.elem+size(),b,elem);
    swap(vb,b);
}

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