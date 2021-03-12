#include "radiosity_string.h"

template<typename Target, typename Source>
Target narrow_cast(Source v)
{
    auto r = static_cast<Target>(v); // convert the value to the target type
    if (static_cast<Source>(r)!=v)
        throw std::runtime_error("narrow_cast<>() failed");
    return r;
}

// Non-Member Functions

char* begin(RadString& x)
{
    return x.c_str();
}

// NOTE(Alex): end always returns pointer to one passed last element!!!
char* end(RadString& x)
{
    return x.c_str()+x.size();
}

const char* begin(const RadString& x)
{
    return x.c_str();
}

const char* end(const RadString& x)
{
    return x.c_str()+x.size();
}

char* expand(const char * ptr, int n)
{
    char * p = new char[n];
    strcpy(p,ptr);
    return p;
}

std::ostream& operator<<(std::ostream& os, const RadString& s)
{
    return os<<s.c_str();
}

std::istream& operator>>(std::istream& is, RadString& s)
{
    s="";
    is>>std::ws;
    char ch=' ';
    while(is.get(ch) && !isspace(ch))
        s += ch;
    return is;
}

bool operator==(const RadString& a, const RadString& b)
{
    if(a.size()!=b.size())
        return false;
    for(int i = 0; i!=a.size();++i)
    {
        if(a[i]!=b[i])
            return false;
    }
    return true;
}

bool operator!=(const RadString& a, const RadString& b)
{
    return !(a==b);
}

RadString& operator+=(RadString& a, const RadString& b)
{
    for(auto x:b)
        a+=x;
    return a;
}

RadString operator+(const RadString& a, const RadString& b)
{
    RadString res{a};
    res+=b;
    return res;
}

RadString operator"" _s(const char* p, size_t)
{
    return RadString{p};
}


// Member Functions

void RadString::copy_from(const RadString & x)
{
    if(x.sz <= short_max)
    {
        memcpy(this, &x, sizeof(x));
        ptr = ch;
    }
    else
    {
        ptr = expand(x.ptr,x.sz + 1);
        sz = x.sz;
        space = 0;
    }
}

void RadString::move_from(RadString & x)
{
    if(x.sz <= short_max)
    {
        memcpy(this,&x, sizeof(x));
        ptr = ch;
    }
    else
    {
        ptr = x.ptr;
        sz = x.sz;
        space = x.space;
        x.ptr = x.ch;
        x.sz  = 0;
        x.ch[0] = 0;
    }
}

RadString::RadString()
: sz{0},ptr{ch}
{
    ch[0] = 0;
}


RadString::RadString(const char*p) :
sz{narrow_cast<int>(strlen(p))},
ptr{(sz<=short_max) ? ch : new char[sz+1]},
space{0}
{
    strcpy(ptr,p);
}

RadString::RadString(const RadString& x)
{
    copy_from(x);
}

RadString::RadString(RadString&& x)
{
    move_from(x);
}

// NOTE(Alex): Strong exception guarantee
RadString& RadString::operator=(const RadString& x)
{
    if(this==&x)return*this;
    char * p=(short_max<sz) ? ptr : 0;
    // NOTE(Alex): In this case we are calling new, therefore can throw so we don´t modify ptr state before throw happpens! 
    copy_from(x);
    delete[] p;
    return *this;
}

RadString& RadString::operator=(RadString&& x)
{
    if(this==&x) return *this;
    if(short_max<sz) delete[] ptr;
    // NOTE(Alex): this movement is sure to not throw. beacuse we are only moving
    move_from(x);
    return *this;
}

RadString& RadString::operator+=(char c)
{
    if(sz==short_max){
        int n = sz+sz+2;
        ptr = expand(ptr,n);
        space = n-sz-2;
    }
    else if(short_max < sz){
        if(space==0){
            int n=sz+sz+2;
            char* p=expand(ptr,n);
            delete[] ptr;
            ptr = p;
            space = n-sz-2;
        }
        else
            --space;
    }
    ptr[sz] = c;
    ptr[++sz] = 0;
    return *this;
}









