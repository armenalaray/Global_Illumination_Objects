/* date = February 17th 2021 7:53 pm */

#ifndef RADIOSITY_MATH_H
#define RADIOSITY_MATH_H


bool IsOdd(size_t HitablePerSide)
{
    return (HitablePerSide & 0x1);
}

class radiosity_float
{
    public:
    float Value;
    
    radiosity_float()
    {
        Value = 0;
    }
    
    radiosity_float(float Value_)
    {
        Value = Value_;
    }
    
    radiosity_float(radiosity_float & Other)
	{
        Value = Other.Value;
	}
    
    radiosity_float operator-() 
    {
        radiosity_float Result(-Value);
        return Result; 
    }
    
    radiosity_float & operator=(const radiosity_float & Other)
    {
        if(this == &Other) return *this;
        Value = Other.Value;
        return *this;
    }
    
    radiosity_float & operator+=(radiosity_float & F)
    {
        Value += F.Value;
        return *this;
    }
    
    radiosity_float operator*(radiosity_float & F)
    {
        radiosity_float Result(Value * F.Value);
        return Result;
    }
    
    radiosity_float operator+(radiosity_float & F)
    {
        radiosity_float Result(Value + F.Value);
        return Result;
    }
    
    radiosity_float operator-(radiosity_float & F)
    {
        radiosity_float Result(Value - F.Value);
        return Result;
    }
    
    radiosity_float operator/(radiosity_float & F)
    {
        radiosity_float Result(Value / F.Value);
        return Result;
    }
    
    bool operator<(radiosity_float & F)
    {
        return (Value < F.Value);
    }
    
    bool operator>(radiosity_float & F)
    {
        return (Value > F.Value);
    }
    
    radiosity_float Inverse()
    {
        radiosity_float Result(1.0f / Value);
        return Result;
    }
    
    friend std::ostream& operator << (std::ostream & S, radiosity_float & F)
    {
        S << F.Value;
        return S;
    }
    
    std::string PrintPPMPixel()
    {
        string Result;
        
        //FFValue /= MaxPixVal;
        int ir = 0;
        int ig = 0;
        int ib = 0;
        
        if(Value < 0)
        {
            ir = 255;
            ig = 0;
            ib = 0;
        }
        else if(Value > 1.0f)
        {
            ir = 0;
            ig = 0;
            ib = 255;
        }
        /*
        else if(IsInSamePlane(i,j))
        {
            ir = 0;
            ig = 255;
            ib = 0;
        }
        */
        else
        {
            ir = int(255.99 * Value);
            ig = int(255.99 * Value);
            ib = int(255.99 * Value);
        }
        
        Result = std::to_string(ir) + " " + to_string(ig) + " " + to_string(ib);
        return Result;
    }
};

template<typename T>
class radiosity_aaquad
{
    public:
    vec2<T> LowBound;
    vec2<T> HighBound;
    
    radiosity_aaquad(vec2<T> LowBound_, vec2<T> HighBound_)
    {
        LowBound = LowBound_;
        HighBound = HighBound_;
    }
    // NOTE(Alex): Contains Lower and Upper bounds!!!
    bool Contains(vec2<T> A)
    {
        bool Result = (A.x <= HighBound.x && A.x >= LowBound.x && 
                       A.y <= HighBound.y && A.y >= LowBound.y);
        return Result;
    }
    
};

template<typename T>
class radiosity_bidimensional_array
{
    public:
    size_t RowCount;
    size_t ColumnCount;
    T ** Data;
    
    radiosity_bidimensional_array()
    {
        RowCount = 0;
        ColumnCount = 0;
        Data = 0;
    }
    
    radiosity_bidimensional_array(size_t RowCount_, size_t ColumnCount_)
    {
        RowCount = RowCount_;
        ColumnCount = ColumnCount_;
        Data = new T*[RowCount];
        for(size_t i = 0; i < RowCount; i += 1)
        {
            Data[i] = new T[ColumnCount];
        }
    }
    
    radiosity_bidimensional_array(radiosity_bidimensional_array<T> & Other)
	{
        RowCount = Other.RowCount;
        ColumnCount = Other.ColumnCount;
        Data = new T*[RowCount];
        for(size_t i = 0; i < RowCount; i += 1)
        {
            Data[i] = new T[ColumnCount];
            for(size_t j = 0; j < ColumnCount; j += 1)
            {
                Data[i][j] = Other[i][j];
            }
        }
	}
    
    radiosity_bidimensional_array & operator=(const radiosity_bidimensional_array & Other)
    {
        if(this == &Other) return *this;
        
        for(size_t i = 0; i < RowCount; i += 1)
        {
            delete [] Data[i];
        }
        delete [] Data;
        
        RowCount = Other.RowCount;
        ColumnCount = Other.ColumnCount;
        
        Data = new T*[RowCount];
        for(size_t i = 0; i < RowCount; i += 1)
        {
            Data[i] = new T[ColumnCount];
            for(size_t j = 0; j < ColumnCount; j += 1)
            {
                Data[i][j] = Other[i][j];
            }
        }
        return *this;
    }
    
    // NOTE(Alex): takes an rvalue reference
    radiosity_bidimensional_array(radiosity_bidimensional_array<T> && Other)
    {
        RowCount = Other.RowCount;
        ColumnCount = Other.ColumnCount;
        Data = Other.Data;
        
        Other.Data = nullptr;
        Other.RowCount = 0;
        Other.ColumnCount = 0;
    }
    
    radiosity_bidimensional_array & operator=(radiosity_bidimensional_array && Other)
    {
        if(this == &Other) return *this;
        
        for(size_t i = 0; i < RowCount; i += 1)
        {
            delete [] Data[i];
        }
        delete [] Data;
        
        RowCount = Other.RowCount;
        ColumnCount = Other.ColumnCount;
        Data = Other.Data;
        
        Other.Data = nullptr;
        Other.RowCount = 0;
        Other.ColumnCount = 0;
        
        return *this;
    }
    
    ~radiosity_bidimensional_array()
    {
        if(Data != nullptr)
        {
            for(size_t i = 0; i < RowCount; i += 1)
            {
                delete [] Data[i];
            }
            delete [] Data;
        }
    }
    
    T * operator [] (size_t i) 
    { 
        return Data[i];
    }
    
    friend std::ostream& operator << (std::ostream & S, radiosity_bidimensional_array<T> & m)
    {
        
        std::ios_base::fmtflags OldFlags = S.flags();
        
        int Width = 12;
        S.precision(5); //Control the number of displayed decimals
        S.setf(std::ios_base::fixed);
        
        for(size_t i = 0; i < m.RowCount; i += 1)
        {
            for(size_t j = 0; j < m.ColumnCount; j += 1)
            {
                S << std::setw(Width) << m[i][j];
            }
            S << endl;
        }
        
        S.flags(OldFlags);
        
        return S;
    }
};


template<typename T>
class matrix : public radiosity_bidimensional_array<T>
{
    public:
    
    matrix(): radiosity_bidimensional_array<T>() {}
    matrix(size_t RowCount_, size_t ColumnCount_) : radiosity_bidimensional_array<T>(RowCount_, ColumnCount_) {}
    
    matrix<T> operator*(matrix<T> & B)
    {
        matrix<T> Result(RowCount, B.ColumnCount);
        size_t ColumnIndex = 0;
        for(;ColumnIndex < B.ColumnCount; ColumnIndex++)
        {
            for (size_t i = 0; i < RowCount; i += 1){
                T Temp;
                for (size_t j = 0; j < ColumnCount; j += 1){
                    Temp += Data[i][j] * B[j][ColumnIndex];
                }
                Result[i][ColumnIndex] = Temp;
            }
        }
        return Result;
    }
    
    template<typename T>
        matrix<T> operator-(matrix<T> & B)
    {
        matrix<T> Result(RowCount, ColumnCount);
        for (size_t i = 0; i < RowCount; i += 1){
            for (size_t j = 0; j < ColumnCount; j += 1){
                Result[i][j] = Data[i][j] - B[i][j];
            }
        }
        return Result;
    }
    
    void MakeDiagonal()
    {
        for (size_t i = 0; i < RowCount; i += 1){
            for (size_t j = 0; j < ColumnCount; j += 1){
                if(i == j)
                {
                    Data[i][j] = T(1.0f);
                }
            }
        }
    }
    
    template<typename T>
        void MakeDiagonal(matrix<T> Other)
    {
        if(Other.ColumnCount == 1)
        {
            for (size_t i = 0; i < RowCount; i += 1){
                for (size_t j = 0; j < ColumnCount; j += 1){
                    if(i == j)
                    {
                        Data[i][j] = Other[i][0];
                    }
                }
            }
        }
    }
    
#if 0    
    template<typename T>
        void ZeroMatrix(matrix<T> * A)
    {
        for (size_t i = 0; i < A->RowCount; i += 1){
            for (size_t j = 0; j < A->ColumnCount; j += 1){
                (*A)[i][j] = 0;
            }
        }
    }
#endif
    
    T SquaredNorm()
    {
        T Result = Dot(*this,*this);
        return Result;
    }
    
    template<typename T>
        T Dot(matrix<T> & A, matrix<T> & B)
    {
        T Result = 0;
        if(A.ColumnCount == 1 && B.ColumnCount == 1)
        {
            for (size_t i = 0; i < A.RowCount; i += 1){
                Result += A[i][0] * B[i][0];
            }
        }
        return Result;
    }
    
    bool IsInSamePlane(size_t i, size_t j)
    {
        bool Result = false;
        if((i / 25) ==  (j / 25))
        {
            Result = true;
        }
        return Result;
    }
    
    friend void NumSolverGS(matrix<T> & A, matrix<T> & x, matrix<T> & b)
    {
        for(size_t i = 0; i < A.RowCount; i += 1)
        {
            T SumT;
            T InverseCoefficient = A.Data[i][i].Inverse();
            for (size_t j = 0; j < A.ColumnCount; j += 1){
                if(j != i)
                {
                    SumT += A.Data[i][j] * x.Data[j][0];
                }
            }
            SumT = -SumT;
            x.Data[i][0] = (b.Data[i][0] + SumT) * InverseCoefficient;
        }
    }
    
    
    void DebugPrint(std::string FileName)
    {
        for(size_t i = 0; i < RowCount; i += 1)
        {
            T Summation;
            for(size_t j = 0; j < ColumnCount; j += 1)
            {
                Summation += Data[i][j];
            }
            cout << i << ": " << "Sum: " << Summation << endl; 
        }
        
        // TODO(Alex): Do we want bigger pixels???
        
        ofstream FFMatrix;
        FFMatrix.open(FileName, ios::trunc | ios::out);
        if(FFMatrix.is_open())
        {
            FFMatrix << "P3\n" << ColumnCount << " " << RowCount << "\n255\n";
            
            for(size_t i = 0; i < RowCount; i += 1)
            {
                for(size_t j = 0; j < ColumnCount; j += 1)
                {
                    //basic_ofstream Temp = Data[i][j].PrintPPMPixel();
                    FFMatrix <<  Data[i][j].PrintPPMPixel() << endl;
                }
            }
            
            FFMatrix.close();
        }
        else cout << "Unable to open file" << endl;
        
    }
    
};

#endif //RADIOSITY_MATH_H
