/* date = March 17th 2021 0:31 pm */

#ifndef MATRIX_H
#define MATRIX_H

template<typename T, size_t N>
class Matrix{
    /* 
An operations taking matrix arguments are provided only if the argument is of Matrix_type

Think of Matrix_ref as a reference to a sub-Matrix.
Matrix_ref<T,0> is specialized so that it refers to a single element.
auto d1=m(1); //error: too few subscripts

Matrix<int,2> m2{
  -   {01,02,03},
  -   {11,12,13},
  -   {21,22,23}
};

auto m22=m2(slice{1,2}, slice{0,3});

then m22

{
  -   {11,12,13},
  -   {21,22,23}
};

or we could do an assignment:
m2(slice{1,2},slice{0,3})={
{111,112,113},
{121,122,123},
};

Matrix<int,2> m3{
  -   {01,02,03},
  -   {11,12,13},
  -   {21,22,23}
};

auto m31=m3(slice{1,2},1); //m31 becomes{{12},{22}}
auto m32=m3(slice{1,2},0); //m32 becomes{{11},{12}}
auto x=m3(1,2) //x becomes{13}

The key difference is that const versions return results with const elements.

OJO: aqui siempre hay un template <> no dos ni tres, etc.
template<typename T,size_t N> es la especificacion de mi template class
template<typename M> es la espcificacion real de mi template function
Enable_if<Matrix_type<M>(),Matrix<T,N>&>  es el concept

What is a Matrix_ref? 
What is a Matrix_slice?
How do you initalize a Matrix from a nest of initializer_lists and make sure the dimensions are reasonable?
How do we ensure that we don´t instantiate a atrix with an unsuitable element type? (concept)!!!! =)))))))))))))))))))))))))



     */
    public:
    // NOTE(Alex): no sabia que se podia initializar un constexpr desde 
    static constexpr size_t order=N; //number of dimensions
    using value_type=T;
    using iterator=typename std::vector<T>::iterator;
    using const_iterator=typename std::vector<T>::const_iterator;
    
    /* 
    The default copy and move operations have just the right semantics: memberwise copy
or move of the desc(slice descriptor defining subscripting and the elements.
Tengo que pensar cuando mis defaults que quiero no son soportados por defualt copy and move.
y default destruct.
     */
    
    Matrix()=default;
    Matrix(Matrix&&)=default;
    Matrix& operator=(Matrix&&)=default;
    Matrix(const Matrix&)=default;
    Matrix& operator=(const Matrix&)=default;
    ~Matrix()=default;
    
    template<typename U>
        Matrix(const Matrix_ref<U,N>&);
    template<typenmae U>
        Matrix& operator=(const Matrix_ref<U,N>&);
    
    template<typename... Exts>                        //specify the extents
        explicit Matrix(Exts... exts);
    
    Matrix(Matrix_initializer<T,N>);                 //initialize from list
    Matrix& operator=(Matrix_initializer<T,N>);      //assing from list                      
    
    // NOTE(Alex): Aqui estas borrando los initializer list defaults!!!
    //a{1,2,3,etc}
    template<typename U>
        Matrix(initializer_list<U>)=delete;          //don´t construct from elements
    //a = {1,2,3,etc}
    template<typename U>
        Matrix& operator=(initializer_list<U>)=delete;
    
    size_t extent(size_t n)const{return desc.extents[n];} // #elements in the nth dimension
    size_t size()const{return elemens.size();}            // total number of elements
    const Matrix_slice<N>& descriptor()const{return desc;} // the slice defining suscripting
    
    T* data(){return elems.data();}                        //"flat" element access
    const T* data()const{return elems.data();}
    template<typename... Args>                     //m(i,j,k) subscripitng with integers
        Enable_if<Matrix_impl::Requesting_element<Args...>(),T&>
        operator()(Args... args);
    template<typename... Args>
        Enable_if<Matrix_impl::Requesting_element<Args...>(),const T&>
        operator()(Args... args) const;
    template<typename... Args>                     //m(s1,s2,s3) subscripting slices
        Enable_if<Matrix_impl::Requesting_slice<Args...>(), Matrix_ref<T,N>>
        operator()(const Args&... args);
    template<typename... Args>
        Enable_if<Matrix_impl::Requesting_slice<Args...>(), Matrix_ref<const T,N>>
        operator()(const Args&... args) const;
    
    Matrix_ref<T,N-1> operator[](size_t i){return row(i);}         // m[i] row access
    Matrix_ref<const T,N-1> operator[](size_t i)const{return row(i);}
    
    Matrix_ref<T,N-1> row(size_t n);                     //row access
    Matrix_ref<const T,N-1> row(size_t n) const;
    
    Matrix_ref<T,N-1> col(size_t n);                     //column access
    Matrix_ref<const T,N-1> col(size_t n) const;
    
    template<typename F>
        Matrix& apply(F f);                              //f(x) for every element x
    
    template<typename M,typename F>                      //f(x,mx) for corresponding elements of *this and m
        Enable_if<Matrix_type<M>(),Matrix&>
        apply(const M& m,F f);
    
    Matrix& operator=(const T& value);                   //assignment with scalar
    Matrix& operator+=(const T& value);
    Matrix& operator-=(const T& value);
    Matrix& operator*=(const T& value);
    Matrix& operator/=(const T& value);
    Matrix& operator%=(const T& value);
    
    template<typename M>
        Enable_if<Matrix_type<M>(),Matrix&>
        operator+=(const M& x);
    
    template<typename M>
        Enable_if<Matrix_type<M>(),Matrix&>
        operator-=(const M& x);
    //...
    private:
    Matrix_slice<N> desc;                                  //slice defining the extents in the dimensions
    // NOTE(Alex): using a vector<T> to hold the elemnts relieves us from concerns of memory management and exception safety.
    vector<T> elems;                                       // the elements
};

// NOTE(Alex): MEMBER

template<typename T,size_t N>
template<typename.. Exts>
Matrix<T,N>::Matrix(Exts... exts):
desc{exts..},                       //copy extents
elems(desc.size)                    //allocate desc.size elements and default initialize them
{}

template<typename T,size_t N>
Matrix<T,N>::Matrix(Matrix_initializer<T,N> init){
    desc.extents=Matrix_impl::derive_extents(init); //deduce extents from initializer list
    Matrix_impl::compute_strides(desc);             //compute strides and size
    elems.reserve(desc.size);                       //make room for slices
    Matrix_impl::insert_flat(init,elems);           //initialize from initializer list
    assert(elems.size()==desc.size);
}

/* 
The use of a template allows us to construct from a Matrix with a compatible element type.
 */
template<typename T,size_t N>
template<typename U>
Matrix<T,N>::Matrix(const Matrix_ref<U,N>& x):
desc{x.dex},elems{x.begin(),x.end()}
{
    static_assert(Convertible<U,T>(),"Matrix constructor: incomplete element types");
}

/* 
Copy asssignment from Matrix_ref
 */
template<typename T, size_t N>
template<typename U>
Matrix<T,N>& 
Matrix<T,N>::(const Matrix_ref<U,N>& x){
    static_assert(Convertible<U,T>(),"Matrix =: incompatible element types");
    desc=x.desc;
    elems.assing(x.begin,x.end());
    return *this;
}

/* 
A Matrix can be accessed through subscripting(to elements or rows), through rows and columns, or through slices(parts of rows or columns).
 Matrix<T,N> Access
m.row(i)     Row i of m; a Matrix_ref<T,N-1>
m.column(i)  Column i of m; a Matrix_ref<T,N-1>
m[i]         C-style subscripting: m.row(i)
 m(i,j)       Fortran-style element access: m[i][j]; a T&;
    -            the number of subscripts must be N!!!
m(slice(i,n),slice(j)) Submatrix access with slicing: a Matrix_ref<T,N>
-            slice(i,n) is elements [i:i+n] of the subscripts dimension
-            slice(j) is elements [j:max) of the subscript's dimension
-            max is the dimension's extent, the number of subscripts must be N
*/


/* 
m[i]         C-style subscripting: m.row(i)
 */

template<typename T, size_t N>
Matrix_ref<T,N-1> 
Matrix<T,N>::operator[](size_t n){
    return row(n);
}

template<typename T,size_t N>
template<typename F>
Matrix<T,N>& 
Matrix<T,N>::apply(F f){
    for(auto& x : elems)f(x);      //this loop uses stride iterators
    return *this;
}

/* 
As usual, returning *this enables chaining. For example:
NOTE(Alex): primero se ejecuta abs y luego sqrt;
m.apply([](double&x){a=abs(x);}).apply([](double& x){a=sqrt(x);}); //m[i]=sqrt(abs(m[i])) for all i
 */

/* 
Scalar Operations
 A scalar arithmetic operation simply applies its operation and right-hand operand to each element.

*/

template<typename T,size_t N>
Matrix<T,N>& 
Matrix<T,N>::operator+=(const T& val){
    // NOTE(Alex): So here is where specify the operation to do!!!
    return apply([&](T& a){a+=val;}); //using a lambda (11.4)
}

/* 
Addition of two matrices
 // NOTE(Alex): M is the type of the argument
which has to be of Matrix_type and has overloaded operator(), and return type Matrix<T,N>&
OJO: esta matrix m, no estoy pidiendo que sea del mismo tipo. solo pido que sea de tipo 
Matrix_type y que tenga el operator() sobrecargado, eso es todo!!!
*/

template<typename T,size_t N>
template<typename M>
Enable_if<Matrix_type<M>(),Matrix<T,N>&> 
Matrix<T,N>::operator+=(const M& m){
    static_assert(m.order==N,"+=: mismatched Matrix dimensions");
    assert(same_extents(desc,m.descriptor())); //make sure iszes match
    return apply(m,[](T& a, const Value_type<M>&b){a+=b});
}

/* 
Matrix::apply(m,f) is the two-argument version of Matrix::apply(f). It applies its f to its two Matrixes
(m and *this) le estas aplicando f.
 */

template<typename T,size_t N>
template<typename M,typename F>
Enable_if<Matrix_type<M>(),Matrix<T,N>&>
Matrix<T,N>::apply(const M& m, F f){
    assert(same_extents(desc,m.descriptor())); //make sure sizes match
    for(auto i=begin(), j=m.begin();i!=end();++i,++j)
        f(*i,*j);
    return *this;
}

namespace Matrix_impl{
    // NOTE(Alex): NON-MEMBER
    
    /* 
    Plain operators 
    Without the move constructor, this return type would be a bad performance bug.
     */
    template<typename T, size_t N>
        inline Matrix<T,N> 
        operator+(const Matrix<T,N>& m, const T& val){
        Matrix<T,N> res=m;
        res+=val;
        return res;
    }
    
    
    /* 
    This defines + for two Matrixes of the same type yielding a result of that type.
     */
    template<typename T,size_t N>
        inline Matrix<T,N> 
        operator+(const Matrix<T,N>& a, const Matrix<T,N>& b){
        Matrix<T,N> res{a};
        res+=b;
        return res;
    }
    
    
    /* 
    We could generalize
     */
    
    template<typename T, typename T2, size_t N, typename RT=Matrix<Common_type<Value_type<T>,Value_type<T2>>,N>>
        inline Matrix<RT,N> 
        operator+(const Matrix<T,N>& a, const Matrix<T2,N>& b){
        Matrix<RT,N> res{a};
        res+=b;
        return res;
    }
    
    
    /* 
    Esto se hace en la resolucion de tipo, para Quad y long double
     */
    template<>
        struct common_type<Quad,long double>{
        using type=Quad;
    };
    
    
    // TODO IMPORTANT(Alex): We need also Matri_refs implementation!!!
    //there is no difference between Matrix and Matrix_ref element access: the difference between Matrix and Matrix_ref is in the initialization and ownership of elements.
    //Subtraction multiplication, etc., by scalars are just repetition of the 
    //techniques used for addition.
    
    template<typename T, size_t N>
        inline Matrix<T,N> 
        operator+(const Matrix_ref<T,N>& x, const T& n){
        Matrix<T,N> res{x};
        res+=n;
        return res;
    }
    
    
    /* 
    // NOTE(Alex): These are specializations of matrix multiplication!!!
    Matrix Multiplication
     N*1 vector by 1*M vector
    // TODO(Alex): Genrealize to accecpt different type for Matrix v!!!
    */
    
    template<typename T>
        inline Matrix<T,2> 
        operator*(const Matrix<T,1>& u, const Matrix<T,1>& v){
        const size_t n=u.extent(0);
        const size_t m=v.extent(0);
        Matrix<T,2>res(n,m);
        for(size_t i=0;i!=n;++i){
            for(size_t j=0;j!=m;++j){
                res(i,j)=u[i]*v[j];
            }
        }
        return res;
    }
    
    
    /* 
    Matrix Multiplication
    N*M Matrix by M*1 vector
     // NOTE(Alex): Note that the declaration of res initializes its elements to T{} which is zero for numeric types.
    */
    template<typename T>
        inline Matrix<T,1> 
        operator*(const Matrix<T,2>& m, const Matrix<T,1>& v){
        assert(m.extent(1)==v.extent(0));
        const size_t nr=m.extent(0);
        const size_t nc=m.extent(1);
        Matrix<T,1> res(nr);
        for(size_t i=0;i!=nr;++i){
            for(size_t j=0;j!=nc;++i){
                res(i)+=m(i,j)*v(j);
            }
        }
        return res;
    }
    
    
    template<typename T>
        inline Matrix<T,2> 
        operator*(const Matrix<T,2>& m1,const Matrix<T,2>& m2){
        const size_t nr=m1.extent(0);
        const size_t nc=m1.extent(1);
        assert(nc==m2.extent(0));
        const size_t p=m2.extent(1);
        Matrix<T,2> res(nr,p);
        for(size_t i=0;i!=nr;++i){
            for(size_t j=0;j!=p;++j){
                for(size_t k=0;k!=nc;++k){
                    res(i,j)+=m1(i,k)*m2(k,j);
                }
            }
        }
        return res;
    }
    
    template<typename M>
        inline Enable_if<Matrix_type<M>(), ostream&>
        operator<<(ostream& os, const M& m)
    {
        os << '{';
        for(size_t i=0;i!=rows(m);++i){
            os << m[i];
            if(i+1!=rows(m)) os << ',';
        }
        return os << '}';
    }
}

/* 
TEST functionality
 */

enum class Piece{none, cross, naught};

Matrix<Piece,2> board1{
    {Piece::none, Piece::none, Piece::none},
    {Piece::none, Piece::none, Piece::none},
    {Piece::none, Piece::none, Piece::none},
};

Matrix<Piece,2> board2(3,3); //OK
Matrix<Piece,2> board3{3,3}; //error:constructor from initializer_list<int> deleted

#endif //MATRIX_H
