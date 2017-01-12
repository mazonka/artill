#pragma once

#include <vector>

class Dlibad
{
    public:
        typedef double (*typef)(int, const double *);
        typef fn;

        typedef std::vector<double> vd;

        Dlibad(typef f): fn(f) {}

        vd solve(const vd & v) const;
};


