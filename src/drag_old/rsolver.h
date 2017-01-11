#pragma once

#include "psic.h"
#include "trres.h"
#include "traj.h"
#include "psolver.h"

class Rsolver
{
        double alpha[2];
        TrType trt[2];

        Psi * psi;

        static const int A = 0, B = 90; // min and max angles

    public:
        typedef std::pair<TrResult, TrType> RsRes;
    private:
        RsRes get(int i) const;
        void init();

    public:
        RsRes getFlat() const { return get(0); }
        RsRes getHigh() const { return get(1); }

        Rsolver(Psi * psi): psi(psi) { init(); }

        void solve(double range, TrType typ) { solve(range, typ, INF_VAL); }
        void solve(double range, TrType typ, double hint);

        Rsolver(Psi * psi, double r, TrType t)
            : psi(psi) { init(); solve(r, t); }

        Rsolver(Psi * psi, double r, TrType t, double h)
            : psi(psi) { init(); solve(r, t, h); }

    private:
        Rsolver(Psi * psi, double range, int);
        Rsolver(Psi * psi, double range, double);
};

