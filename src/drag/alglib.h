#pragma once

#include "asolver.h"

class Alglib : public Asolver
{
    public:
        Alglib() {}
        Alglib(AsolFun * fn, const Params & p): Asolver(fn, p) {}
        Params solve();
};

void testAlglibMin(Asolver * a);
