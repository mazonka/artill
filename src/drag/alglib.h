#pragma once

#include "asolver.h"

class Alglib : public Asolver
{
    public:
        Alglib() {}
        Alglib(AsolFun * fn, const Params & p): Asolver(fn, p) {}

        // switch to disable dlib
        //Params solve();
        Params solve(){ return Params(); }
};

