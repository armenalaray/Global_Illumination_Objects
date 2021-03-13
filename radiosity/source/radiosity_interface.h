/* date = March 12th 2021 9:28 am */

#ifndef RADIOSITY_INTERFACE_H
#define RADIOSITY_INTERFACE_H


/* 
 The idea is to ahve a class IvaI_box(integer value input box) that knows what range of input values it will accept. 
A program can ask an lval_box for its value. and ask it to prompt the user if necessary.
Hasta le puedes mandar comandos al user. es un intermediario!!!

Puedes hacer todo tu sistema asi por medio de intermediarios!!!

Asi mi sistema no sabe como se calcula blah pero puede pedir informacion y mandar comandos a otra clase por medio de mensajes.


El programa no debe de saber nada de como promptear al user ni cuales son los valores limite, ni que tipos de valores utilizar 
solo debe de tener una interfaz a la cual preguntarle blah, blah, blah

Entonces la interfaz es usada por el programa y por el usuario ooh!!
Jamas había utilizado una clase que fuera utilizada por dos clases

*/

class Old_IvaI_box{
    public:
    
    Old_IvaI_box(int ll, int hh): val{ll}, low{ll}, high{hh}{}
    
    virtual int get_value(){ changed = false; return val;}  //for application
    virtual int set_value(int i){ changed = true; val = i;}  //for user
    virtual void reset_value(int i){ changed = false; val = i;}  //for application
    virtual void prompt(){}
    
    // NOTE(Alex): with const member function you asume that all members are const
    virtual bool was_changed() const { return changed; }
    
    virtual ~Old_IvaI_box(){}
    
    protected:
    
    int val;
    int low, high;
    bool changed{false};      //changed by user using set_value()
};


class Old_IvaI_slider : public Old_IvaI_box {
    public:
    
    Old_IvaI_slider(int,int);
    // NOTE(Alex): Note how i don´t need to override all of them because they are not pure virtual functions
    int get_value() override; //get value from user and deposit it in val
    void prompt() override; 
    
    private:
    // graphics stuff to define what the slider looks like, etc ...
};


/* 
The user-interface system should be an implementation detail that is hidden from users who don´t want to know about it.
The IvaI_box class should not contain data.
No recompilation of code using the IvaI_box family of calsses should be required after a change of the user-interface system.
IvaI_boxes for diffetent interface systems should be able to coexist in our program.
Instead i added a virtual destructor to ensure proper cleanup of the data that will be defined in the derived classes.
*/

// NOTE(Alex): abstract class
class IvaI_box{
    public:
    // NOTE(Alex): the constructor is gone since no data for it to initialize.
    virtual int get_value()=0;
    virtual void set_value(int i) =0;
    virtual void reset_value(int i)=0;
    virtual void prompt()=0;
    virtual bool was_changed()const=0;
    virtual ~IvaI_box(){}
};

#if 0
class IvaI_slider:public IvaI_box, protected BBwidget{
    public:
    IvaI_slider(int,int);
    ~IvaI_slider() override;
    
    int get_value() override;
    void set_value(int i) override;
    // ...
    protected:
    // ... functions overriding BBwidget virtual functions
    // e.g., BBwidget::draw(), BBwidget::mouse1hit() ...
    private:
    // ... data needed for slider ...
};
#endif

class Vector{
    public:
    
    Vector::Vector(int s)
    {
        if(s<0)throw std::bad_array_new_length{s};
        sz=s;
        elem=new double[s];
    }
    
    private:
    double*elem; // elem points to an array of sz doubles
    int sz;      // sz is non-negative
};







#endif //RADIOSITY_INTERFACE_H
