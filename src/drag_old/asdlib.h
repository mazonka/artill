#pragma once

#include "asolver.h"

class Asdlib : public Asolver
{
    public:
        Asdlib() {}
        Asdlib(AsolFun * fn, const Params & p): Asolver(fn, p) {}
        Params solve();
};

