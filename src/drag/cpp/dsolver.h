#pragma once

#include "psic.h"
#include "dataset.h"

class Dsolver
{
        const Dataset * ref;

    public:
        Psi * psi;
        Dataset * data;

        ~Dsolver() { delete data; }
        Dsolver(Psi * p, const Dataset * d);

        void solve(double smcoeff);
        double u() const { return data->util(ref); }

        Dataset * ds() { return data; }

    private:
        Dsolver(const Dsolver &);
        void operator=(const Dsolver &);
};


void strategic_solver();
