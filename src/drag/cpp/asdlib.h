#pragma once

#include "asolver.h"

class Asdlib : public Asolver
{
    public:
        ///Asdlib() {}
        Asdlib(AsolFun * fn, const Params & p, const void * d)
            : Asolver(fn, p, d) {}

        // switch to disable dlib
        Params solve(int maxeval);
        //Params solve(){ return Params(); }
};

