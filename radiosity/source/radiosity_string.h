/* date = March 10th 2021 11:16 am */

#ifndef RADIOSITY_STRING_H
#define RADIOSITY_STRING_H

// NOTE(Alex): zero terminated arrays of characters
// NOTE(Alex): short string optimization

/*
 Essential operations
value semantics, after s1=s2 operations on s1 does not affect s2 
 
conventional notation
maximally efficient
range checked

Al string members take care not to modify a new representation before they are certain that a new one can be put in place.
In particular they don´t delete until after any possible new operations have been done.

*/

#include <stdexcept>
#include <ostream>
#include <istream>

class RadString
{
    /* 
    A simple string that implements the short string optimization
    
size()==sz is the nimber of elements
if size()<=short_max, the characters are held in the String object itself,
otherwise the free store is used.

ptr points to the start of the character sequence
the character sequence is kept zero-terminated: ptr[size()]==0;
this allows us to use C library string functions and to easily return a C-style string:c_str()

To allow efficient addition of characters at end, String grows by doubling its allocation; 
capacity() is the amount of space available for characters (excluding the terminating 0): sz+space 
     
parece ser que cuando defino una function miembro const hace que los miembros se hagna const
*/
    public:
    RadString();
    RadString(const char*p);
    RadString(const RadString&);
    RadString& operator=(const RadString&);
    
    RadString(RadString&& x);
    RadString& operator=(RadString&& x);
    
    ~RadString() { if(short_max<sz) delete[]ptr; }
    
    char& operator[](int n) { return ptr[n]; } //unchecked element access
    char operator[](int n) const { return ptr[n]; }
    
    char & at(int n) { check(n); return ptr[n]; }  // range-checked element access
    char at(int n) const { check(n); return ptr[n]; } 
    
    RadString& operator+=(char c);
    // TODO(Alex): add RadString& operator+=(char* p);?
    // NOTE(Alex): s2 += "and high water"; this works beacuse is like this: 
    //s2 += RadString("and high water");
    
    char* c_str() { return ptr; } //C style string access
    const char* c_str() const { return ptr; }
    
    int size() const { return sz; }
    int capacity() const 
    {
        return(sz<=short_max) ? short_max: sz+space;
    }
    
    private:
    
    static const int short_max = 15;
    int sz;
    char * ptr;
    
    union
    {
        int space;
        char ch[short_max+1];
    };
    
    void check(int n) const //range check
    {
        if(n<0 || sz<=n)
            throw std::out_of_range("String::at()");
    }
    
    // ancillary member functions:
    void copy_from(const RadString & x);
    void move_from(RadString & x);
    
};

char* begin(RadString& x);
char* end(RadString& x);
const char* begin(const RadString& x);
const char* end(const RadString& x);

char* expand(const char * ptr, int n);

std::ostream & operator<<(std::ostream& os, const RadString& s);
std::istream& operator>>(std::istream& is, RadString& s);
bool operator==(const RadString& a, const RadString& b);
bool operator!=(const RadString& a, const RadString& b);

RadString& operator+=(RadString& a, const RadString& b);
RadString operator+(const RadString& a, const RadString& b);
RadString operator"" _s(const char* p, size_t);


#endif //RADIOSITY_STRING_H
