// Trajectory Result

#pragma once

#include <vector>

#include "fpoint.h"
#include "psic.h"
#include "util.h"

struct TrResult
{
    void init();

    TrResult() { init(); }
    TrResult(const Psi * psi, const std::vector<Fpoint> & path);
    void save(bool append) const;

    double tim; // time
    double range; // range
    double ek0, ekf; // initial and final kinetic
    double heatKJ; // final heat
    double length; // trajectory length
    double v0, vf; // initial and final velocities
    double alpha, beta; // shot and fall angles
    double height0; // initial shot height
    double topHeight, topRange; // height and range of the top (not X/Y max)
    // hieght is distance from the earth surface

    double finalZ; // perpendicular deviation
    double rej; // rejection rate
};

struct TrType
{
        enum Type { UNKNOWN = 0, FLAT = 1, HIGH = 2, MAX = 3 } type;
        string str() const;

        TrType() : type(UNKNOWN) {}
        TrType(Type t) : type(t) {}
        explicit TrType(double x) : type(Type(int(x + 0.1))) {}

        double dbl() const { return type; }

    private:
        TrType(int);
};

struct TrSolution
{
    TrResult r;
    TrType t;
    double hint;
    TrSolution(TrResult r, TrType t, double h): r(r), t(t), hint(h) {}
};
