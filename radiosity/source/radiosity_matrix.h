/* date = March 9th 2021 2:37 pm */

#ifndef RADIOSITY_MATRIX_H
#define RADIOSITY_MATRIX_H

#include <array>
#include "radiosity_vector_base.h"
/* 
A typical implementation of vector(4.4.1,31.4) will consist of a handle holding pointers to the first element,
one past-the last element, and one-past the last allocated space or the equivalent information represented as a pointer plus offsets):

In addition, it holds an allocator(here, alloc) from which the vector can aquire memory for its elements. 
The default allocator(34.4.1) uses new and delete to acquire and release memory.

Here is a declaration of vector 
 */

/* 
This style of consructor relies on the fundamental language rule that when an exception is thrown from a constructor, subobjects(including bases)
that have already been completely constructed will be properly destruyed.
the uninitialized_fill()  algorithm and its cousins(13.6.1) provide the uquivalent guarantee for partially constructed sequences.
 Avoid throwing move operators as you avoid throwing destructors (13.2, 17.2.2)
*/

template<typename T, typename A=allocator<T>>
class vector{
    public:
    using size_type=typename A::size_type;  //type used for vector sizes
    explicit vector(size_type n, const T& val=T{}, const A& a=A{});
    vector(const vector&);
    vector& operator=(const vector&);
    vector(vector&&);
    vector& operator=(vector&&);
    ~vector(){destroy_elements();}
    size_type size() const{return vb.space-vb.elem;}
    size_type capacity() const{return vb.last-vb.elem;}
    void reserve(size_type);
    void resize(size_type,const T& ={});
    void clear(){resize(0);}
    void push_back(const T&);
    // ...
    private:
    vector_base<T,A> vb;
    void destroy_elements();
};

template<typename T> void sort(vector<T>&);        //declaration
template<typename In> void destroy(In b, In e);

#endif //RADIOSITY_MATRIX_H
