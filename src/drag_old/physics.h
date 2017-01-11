#pragma once

#include "fpoint.h"

struct Drag { double gamma, rho; };

struct Psi;
Drag getDrag(const Psi * psi, const Fpoint & y);
void applyDrag(const Psi * psi, const Fpoint & y1, Fpoint & y2, Drag);
void applyKick(const Psi * psi, const Fpoint & y1, Fpoint & y2, Drag);
std::pair<double, double> air_rho(double Z);

double dEp(Fpoint a, Fpoint b);
double dist(Fpoint a, Fpoint b);
double rangeR(Fpoint a);
Fpoint rangeP(Fpoint a);
bool renormE(Fpoint & p, double e);

void tracef(const char * L, const Fpoint & y1, const Fpoint & y2,
            double dt, double dE, double dK, double dP);

inline double Ek(Fpoint a)
{
    double ea = a.vx * a.vx + a.vy * a.vy + a.vz * a.vz;
    return ea / 2;
}

inline double dEk(Fpoint a, Fpoint b)
{
    double ea = a.vx * a.vx + a.vy * a.vy + a.vz * a.vz;
    double eb = b.vx * b.vx + b.vy * b.vy + b.vz * b.vz;
    return (eb - ea) / 2;
}

inline double dEkp(Fpoint a, Fpoint b) { return dEk(a, b) + dEp(a, b); }

