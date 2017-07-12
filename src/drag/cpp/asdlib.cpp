#include <iostream>
#include <cmath>

#include "dlibad.h"

#include "asdlib.h"

typedef std::vector<double> vd;

AsolFun * asol_fun;
double as_fun(int sz, const double * v, const void * d)
{
    return asol_fun->f(Params(sz, v), d);
}

Params Asdlib::solve()
{
    asol_fun = f;
    f->f(p, data );

    Dlibad d(as_fun, data);
    p.v = d.solve(p.v);

    return p;
}

