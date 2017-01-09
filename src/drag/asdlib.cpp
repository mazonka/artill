#include <iostream>
#include <fstream>
#include <cmath>

#include "dlibad.h"

#include "asdlib.h"

typedef std::vector<double> vd;

AsolFun * asol_fun;
double as_fun(int sz, const double * v)
{
	return asol_fun->f(Params(sz,v));
}

Params Asdlib::solve()
{
	asol_fun = f;
	f->f(p);

	Dlibad d(as_fun);
	p.v = d.solve(p.v);

    return p;
}

