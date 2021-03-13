#include <iostream>
using namespace std;
#include "radiosity_string.h"


/* 
YOUAREHERE: 17.4.1 Member Initialization
*/



/* 
#if 0
#include <iostream>
#include "radiosity_matrix.h"

#define GARBAGE_COLLECTION

#if defined(SHALLOW_COPY)
struct S
{
    int* p; // a pointer
};

S x{new int{0}};

void f()
{
    S y{x};          //"copy" x
    *y.p = 1;        //change y:affects x
    *x.p = 2;        //change x:affects y
    delete y.p;      //affects x and y
    y.p = new int{3}; //OK: change y;does not affect x
    *x.p=4;           //oops:write to deallocated memory
}
#elif defined(GARBAGE_COLLECTION)

struct S
{
    std::shared_ptr<int> p; // a pointer
};

S x{std::shared_ptr<int>{new int{0}}};

void f()
{
    S y{x};          //"copy" x
    *y.p = 1;        //change y:affects x
    *x.p = 2;        //change x:affects y
    y.p.reset(new int{3}); //OK: change y;does not affect x
    *x.p=4;           //oops:write to deallocated memory
}

#endif

constexpr int rc_max {4};//row and column size

class Matrix;

class Vector
{
    float v[rc_max];
    friend Vector operator*(const Matrix&, const Vector&);
};

class Matrix
{
    Vector v[rc_max];
    friend Vector operator*(const Matrix&, const Vector&);
};

Vector operator*(const Matrix& m, const Vector& v)
{
    Vector r;
    for(int i=0;i!=rc_max;++i)
    {
        r.v[i] = 0;
        for(int j=0;j!=rc_max;++j)
            r.v[i] += m.v[i].v[j] * v.v[j];
    }
    return r;
}

class List_iterator
{
    int * next();
};

class List
{
    // NOTE(Alex): HERE i make all List_iterator memeber functions friends of List
    friend class List_iterator;
};

int main()
{
    
#if 0    
    Matrix a 
    {
        {0,0,0,0},
        {0,0,0,0},
        {0,0,0,0},
        {0,0,0,0},
    };
    
    Vector b {0,0,0,0};
#endif
    
    
    //f();
#if 0    
    radiosity_matrix<double, 0> m0{1}; //zero dimensions: a scalar
    radiosity_matrix<double, 1> m1{1,2,3,4}; //one dimension: a vector(4,elements)
    radiosity_matrix<double, 2> m2{
        {00,01,02,03}, //row0 
        {10,11,12,13}, //row1
        {20,21,22,23}, //row2
    };
    
    radiosity_matrix<double,3> m3(4,7,9); //three dimenisons(4*7*9), all 0-initialized
    radiosity_matrix<complex<double>,17> m17; //17 dimendions (no elements so far)
    
    radiosity_matrix<double,2> md;
    radiosity_matrix<string,2> ms; //OK: just don´t try arithmetic operations
    
    radiosity_matrix<radiosity_matrix<int,2>,3> mm
    {
        {//row 0
            {{1,2},{3,4}}, //col 0
            {{4,5},{6,7}}, //col 1
        },
        {//row 1
            {{1,2},{3,4}}, //col 0
            {{4,5},{6,7}}, //col 1
        },
        {//row 2
            {{1,2},{3,4}}, //col 0
            {{4,5},{6,7}}, //col 1
        }
    };
    
    radiosity_matrix<char,2> mc1(2,3,4); //error: too many dimension sizes
    radiosity_matrix<char,2> mc2 //error: initializer missing for second dimension
    {
        {'1','2','3'}
    };
    
    radiosity_matrix<char,2> mc3 //error: element missing for third column
    {
        {'1','2','3'}
        {'1','2'}
    };
    
    
    radiosity_matrix<double, 1> m1(100); //one dimension: a vector(100 elements)
    radiosity_matrix<double, 2> m2(50,6000); //two dimensions: 50*6000 elements
    
    auto d1 = m1.order; //1 Orders are dimensions
    auto d2 = m2.order; //2
    
    auto e1 = m1.extent(0); // 100
    auto e2 = m1.extent(1); // error: m1 is one-dimensional
    
    auto e3 = m2.extent(0); // 50
    auto e4 = m2.extent(1); // 6000
    
    auto s1 = m1.size(); //100
    auto s2 = m2.size(); //50*6000
    
    radiosity_matrix<double, 2> m{ //two dimensions (3*4 elements)
        {00,01,02,03}, //row0 
        {10,11,12,13}, //row1
        {20,21,22,23}, //row2
    };
    
    
    double d1 = m(1,2); // d==12
    double d2 = m[1][2]; // d==12
    radiosity_matrix<double,1> m1 = m[1]; // row 1: {10,11,12,13}
    double d3 = m1[2];  //d==12
#endif
}
#endif


int hash(const RadString& s)
{
    if(s.size()==0) return 0;
    int h {s[0]};
    for(int i{1}; i<s.size();++i)
        h ^= s[i]>>1;
    return h;
}

#if 0
void print_in_order(const RadString & s, const vector<int>& Index)
{
    // NOTE(Alex): you need this to run index times but s is limited so do inline testing 
    for(auto x:index)
        const << s.at(x) << '\n'; //checked access to s
}
#endif

void f(const char*); //C-style string
void f(const RadString&); //our string 

int main()
{
    RadString s("abcdefghij");
    cout << s << '\n';
    s += 'k';
    s += 'l';
    s += 'm';
    s += 'n';
    cout << s << '\n';
    
    RadString s2 = "Hell";
    s2 += "and high water";
    cout << s2 << '\n';
    
    RadString s3 = "qwerty";
    s3 = s3;
    RadString s4 = "the quick brown fox jumped over the lazy dog";
    s4 = s4;
    cout << s3 << " " << s4 << "\n";
    cout << s + "." + s3 + RadString(". ") + "Horsefeathers\n";
    
    RadString buf;
    while(cin>>buf && buf!="quit")
        cout << buf << buf.size() << " " << buf.capacity() << '\n';
    
    //f("Madden's");
    //f("Christopher's"_s);
}
 */

int main()
{
    
}

