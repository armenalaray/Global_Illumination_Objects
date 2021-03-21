/* date = March 20th 2021 0:12 pm */

#ifndef OUTPUT_H
#define OUTPUT_H

#include <stdexcept>
#include <iostream>

namespace Output_lib{
    
    /* 
    NOTE that *++s does not overflow since s ponts to null terminated string
     */
    void printf(const char*s){
        if(s==nullptr)return;
        while(*s){
            if(*s=='%' && *++s!='%')
                throw std::runtime_error("invalid_format: missing arguments");
            std::cout << *s++;
        }
    }
    
    // NOTE(Alex): Variadic template argument list: one or more arguments
    template<typename T, typename... Args>
        // NOTE(Alex): Function template argument list: two or more arguments
        void printf(const char * s, T value, Args... args){
        while(s && *s){
            if(*s=='%' && *++s!='%'){
                if(*s==0){
                    throw std::runtime_error("Invalid format");
                }
                std::cout<<value;
                return printf(++s,args...);      //do a recursive cal 
            }
            std::cout << *s++;
        }
        throw std::runtime_error("extra arguments provided to printf");
    }
    
#if 0
    template<typename T, typename... Args>
        void printf(const char* s, T value, Args... args){
        while(s &&*s){
            if(*s=='%'){
                switch(*++s){
                    case '%':
                    break;
                    case 's':
                    {
                        if(!Is_C_style_string<T>() && !is_string<T>())
                            throw runtime_error("Bad printf() format");
                    }break;
                    case 'd':
                    {
                        if(!Is_integral<T>()) throw runtime_error("Bad printf() format");
                    }break;
                    case 'g':
                    {
                        if(!is_floating_point<T>()) throw runtime_error("Bad printf() format");
                    }break;
                }
                std::cout<<value;
                return printf(++s,args...);
            }
            std:.cout<<*s++;
        }
        throw::std_runtime_error("extra arguments provided to printf");
    }
#endif
}

#endif //OUTPUT_H
