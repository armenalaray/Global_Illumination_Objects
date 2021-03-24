/* date = March 24th 2021 2:08 pm */

#ifndef MAPPED_QUAD_H
#define MAPPED_QUAD_H

#include "vec3.h"
#include "quad.h"

class Mapped_quad{
    public:
    Mapped_quad():
    qp{}
    {}
    Mapped_quad(std::shared_ptr<Quad> oqp):
    qp{oqp}
    {}
    virtual void add_color(const Color<float>& c)=0;
    protected:
    std::shared_ptr<Quad> qp;
};

class Mapped_quad_ur:public Mapped_quad{
    public:
    Mapped_quad_ur(){}
    Mapped_quad_ur(std::shared_ptr<Quad> p):Mapped_quad{p}{}
    void add_color(const Color<float>& c)override;
    private:
};

class Mapped_quad_u:public Mapped_quad{
    public:
    Mapped_quad_u(){}
    Mapped_quad_u(std::shared_ptr<Quad> p):Mapped_quad{p}{}
    void add_color(const Color<float>& c)override;
    private:
};

class Mapped_quad_ul:public Mapped_quad{
    public:
    Mapped_quad_ul(){}
    Mapped_quad_ul(std::shared_ptr<Quad> p):Mapped_quad{p}{}
    void add_color(const Color<float>& c)override;
    private:
};

class Mapped_quad_r:public Mapped_quad{
    public:
    Mapped_quad_r(){}
    Mapped_quad_r(std::shared_ptr<Quad> p):Mapped_quad{p}{}
    void add_color(const Color<float>& c)override;
    private:
};

class Mapped_quad_k:public Mapped_quad{
    public:
    Mapped_quad_k(){}
    Mapped_quad_k(std::shared_ptr<Quad> p):Mapped_quad{p}{}
    void add_color(const Color<float>& c)override;
    private:
};

class Mapped_quad_l:public Mapped_quad{
    public:
    Mapped_quad_l(){}
    Mapped_quad_l(std::shared_ptr<Quad> p):Mapped_quad{p}{}
    void add_color(const Color<float>& c)override;
    private:
};

class Mapped_quad_br:public Mapped_quad{
    public:
    Mapped_quad_br(){}
    Mapped_quad_br(std::shared_ptr<Quad> p):Mapped_quad{p}{}
    void add_color(const Color<float>& c)override;
    private:
};

class Mapped_quad_b:public Mapped_quad{
    public:
    Mapped_quad_b(){}
    Mapped_quad_b(std::shared_ptr<Quad> p):Mapped_quad{p}{}
    void add_color(const Color<float>& c)override;
    private:
};

class Mapped_quad_bl:public Mapped_quad{
    public:
    Mapped_quad_bl(){}
    Mapped_quad_bl(std::shared_ptr<Quad> p):Mapped_quad{p}{}
    void add_color(const Color<float>& c)override;
    private:
};


#endif //MAPPED_QUAD_H
