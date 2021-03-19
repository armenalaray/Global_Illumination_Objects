/* date = March 17th 2021 0:31 pm */

#ifndef MATRIX_H
#define MATRIX_H

template<typename T, size_t N>
class Matrix{
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
    
    private:
    Matrix_slice<N> desc;                                  //slice defining the extents in the dimensions
    // NOTE(Alex): using a vector<T> to hold the elemnts relieves us from concerns of memory management and exception safety.
    vector<T> elems;                                       // the elements
};

template<typename M>
Enable_if<Matrix_type<M>(), ostream&>
operator<<(ostream& os, const M& m)
{
    os << '{';
    for(size_t i=0;i!=rows(m);++i){
        os << m[i];
        if(i+1!=rows(m)) os << ',';
    }
    return os << '}';
}

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
Matrix<T,N>& Matrix<T,N>::(const Matrix_ref<U,N>& x){
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
Matrix_ref<T,N-1> Matrix<T,N>::operator[](size_t n){
    return row(n);
}



enum class Piece{none, cross, naught};

Matrix<Piece,2> board1{
    {Piece::none, Piece::none, Piece::none},
    {Piece::none, Piece::none, Piece::none},
    {Piece::none, Piece::none, Piece::none},
};

Matrix<Piece,2> board2(3,3); //OK
Matrix<Piece,2> board3{3,3}; //error:constructor from initializer_list<int> deleted

#endif //MATRIX_H
