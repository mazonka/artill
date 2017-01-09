#include <iostream>
#include <fstream>
#include <cmath>

#include "dlibad.h"

#include "asdlib.h"

typedef std::vector<double> vd;

Params Asdlib::solve()
{
    //xmn(p.v, f);
	f->f(p);
    return p;
}

