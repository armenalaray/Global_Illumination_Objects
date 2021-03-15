/* date = March 14th 2021 2:29 pm */

#ifndef RADIOSITY_XREF_H
#define RADIOSITY_XREF_H

#include <utility>
#include <string>

template<typename T>
class Xref{
    public:
    Xref(int i, T*p): //store a pointer: Xref is the owner
    index{i},elem{p},owned{true}{}
    
    Xref(int i, T& r): //store a pointer r, owned by someone else
    index{i},elem{&r},owned{false}{}
    
    Xref(int i, T&& r):   //move r into Xref,Xref is the owner
    index{i}, elem{new T{std::move(r)}}, owned{true}{}
    
    ~Xref()
    {
        if(owned)delete elem;
    }
    //...
    private:
    int index;
    T* elem;
    bool owned;
};

void xref_f()
{
    std::string x{"There and back again"};
    Xref<std::string> r1{7,"Here"};            //r1 owns a copy of string{"Here"}
    Xref<std::string> r2{9,x};                 // r2 just refers to x
    Xref<std::string> r3{3,new std::string{"There"}}; //r2 owns the string{"There"}
    
}


#endif //RADIOSITY_XREF_H
