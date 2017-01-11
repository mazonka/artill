#pragma once

#include <vector>

struct Params
{
    std::vector<double> v;
    Params() {}
    Params(int z, const double * p) : v(p, p + z) {}
    const double * data() const { return v.data(); }
};

struct AsolFun
{
    virtual double f(const Params &) = 0;
};

class Asolver
{
    protected:
        AsolFun * f;
        Params p;

    public:
        Asolver(): f(nullptr) {}
        Asolver(AsolFun * fn, const Params & p): f(fn), p(p) {}
        virtual Params solve() = 0;
        void setFun(AsolFun * af) { f = af; }
        void setParams(const Params & ap) { p = ap; }

    private:
        Asolver(const Asolver &);
        void operator=(const Asolver &);
};

void testAsolver(Asolver * a);

Asolver * make_solver(AsolFun * fn, const Params & p);
