#include <iostream>
#include <fstream>
#include <cmath>

#include "optimization.h" // ALGLIB

#include "alglib.h"
#include "asolver.h"

typedef std::vector<double> vd;

void fun1fun(const alglib::real_1d_array & x, double & result, void * ptr)
{
    AsolFun * f = (AsolFun *)(ptr);
    int sz = x.length();
    const double * p = x.getcontent();
    result = f->f(Params(sz, p));
}

void xmn(vd & v, AsolFun * fn)
{
    alglib::real_1d_array x;
    x.setcontent(v.size(), v.data());

    double epsf = 0;        // 1
    double epsx = 0;        // 2
    double epsg = 0.0000001;    // 4
    double diffstep = 1.0e-4;
    alglib::ae_int_t maxits = 0;
    alglib::minlbfgsstate state;
    alglib::minlbfgsreport rep;

    alglib::minlbfgscreatef(1, x, diffstep, state);
    alglib::minlbfgssetcond(state, epsg, epsf, epsx, maxits);
    alglib::minlbfgsoptimize(state, fun1fun, NULL, (void *)fn);
    alglib::minlbfgsresults(state, x, rep);

    printf("\nalglib term %d\n", int(rep.terminationtype)); // EXPECTED: 4
    printf("%s\n", x.tostring(2).c_str()); // EXPECTED: [-3,3]

    int sz = v.size();
    for (int i = 0; i < sz; i++)
        v[i] = x[i];
}

Params Alglib::solve()
{
    //p.v[6] += 1.1;
    xmn(p.v, f);
    return p;
}

