#pragma once

#include <vector>

class Dlibad
{
    public:
        typedef double (*typef)(int, const double *, const void *);
        typef fn;
        const void * data;

        typedef std::vector<double> vd;

        Dlibad(typef f, const void * d): fn(f), data(d) {}

        vd solve(const vd & v, int maxeval) const;
};


///extern int Qmaxeval; // 100000;

