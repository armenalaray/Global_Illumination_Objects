#include "radiosity_algorithm.h"

void ssort(void*base, size_t n, size_t sz, CFT cmp)
/* 
Sort the "n" elements of vector "base" into increasing order
using the comparison function pointed to by "cmp".
the elements are of size "sz".

Shell sort (knuth, vol3, pg84)
 */
{
    for(int gap=(int)n/2; 0<gap;gap/=2){
        for(int i=gap;i!=n;i++){
            for(int j=i-gap;0<=j;j-=gap){
                char* b=static_cast<char*>(base);   //necessary cast
                char* pj=b+j*sz;                    //&base[j]
                char* pjg=b+(j+gap)*sz;             //&base[j+gap];
                if(cmp(pjg,pj)<0){
                    for(int k=0; k!=sz; k++){
                        char temp=pj[k];
                        pj[k]=pjg[k];
                        pjg[k]=temp;
                    }
                }
            }
        }
    }
}

template<typename T1, typename T2>
std::pair<T1,T2> make_pair_a(T1 a, T2 b)
{
    // NOTE(Alex): this calls the initializer for pair<T1,T2> returnV {};
    //int this way i can make:
    return {a,b};
}

void f()
{
    auto x = make_pair_a(1,2);
    auto y = make_pair_a(std::string("New York"),7.7);
}