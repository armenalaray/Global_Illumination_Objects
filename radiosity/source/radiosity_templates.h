/* date = March 15th 2021 4:29 pm */

#ifndef RADIOSITY_TEMPLATES_H
#define RADIOSITY_TEMPLATES_H

/* 
The templatem argument name, T, is only accesible to the template itself, 
so for other code to refer to the element type, we must provide an alias.
Esto se usa para los conceptos!!!

I need to use type aliases to make public the template arguments to other classes!!! and to make concepts!!!

A random access iterator can go directly to an element:

Aah osea le metes Iter en typename iterator_traits<Iter>::iterator_category{}
y esa instancia tiene uno de los diferentes tipos de categorias para iterators.

advance_helper esto avanza de muschas maneras, aha!!!

so if we want to know the type of the difference between two iterators(pointing to the same sequence)

*/

template<typename iter>
struct iterator_traits{
    using value_type=typename iter::value_type;
    using difference_type=typename iter::difference_type;
    using pointer=typename iter::pointer;
    using reference=typename iter::reference;
    using iterator_category=typename iter::iterator_category;
}

template<typename T>
struct iterator_traits<T*>{ //specialization for pointers
    using difference_type=ptrdiff_t;
    using value_type=T;
    using pointer=T*;
    using reference=T&;
    using iterator_category= random_access_iterator_tag;
};

template<typename T>
class Vector{
    public:
    using value_type = T;
    using iterator = Vector_iter<T>;
};


#endif //RADIOSITY_TEMPLATES_H
