#pragma once

#include <vector>

///#include "func.h"

struct Params
{
    std::vector<double> v;
    Params() {}
    Params(int z, const double * p) : v(p, p + z) {}
    const double * data() const { return v.data(); }
};

struct AsolFun
{
    virtual double f(const Params &, const void * data) = 0;
};

class Asolver
{
    protected:
        AsolFun * f;
        Params p;
        const void * data;

    public:
        Asolver(): f(nullptr) {}

        Asolver(AsolFun * fn, const Params & p, const void * d)
            : f(fn), p(p), data(d) {}

        virtual Params solve(int maxeval) = 0;
        void setFun(AsolFun * af) { f = af; }
        void setParams(const Params & ap) { p = ap; }

    private:
        Asolver(const Asolver &);
        void operator=(const Asolver &);
};

void testAsolver(Asolver * a);

Asolver * make_solver(AsolFun * fn, const Params & p, const void * data);
