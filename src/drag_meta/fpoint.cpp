#include "psic.h"
#include "fpoint.h"

Fpoint Fpoint::next(double h) const
{
    const Fpoint & y = *this;
    Fpoint k1 = f(y);

    Fpoint k2 = f(y + 0.5 * h * k1);
    Fpoint k3 = f(y + 0.5 * h * k2);
    Fpoint k4 = f(y + h * k3);

    Fpoint yn = y + (h / 6) * (k1 + 2 * k2 + 2 * k3 + k4);
    yn.t = y.t + h;
    return yn;
}

Fpoint Fpoint::f(Fpoint a)
{
    static const double GM = cst::Ggrav * cst::Earth_mass;

    double ry = cst::Earth_radius + a.qy;

    double r = a.R1();
    double r2 = a.R2();
    double ff = GM / r2;

    double avx = -a.qx * ff / r;
    double avy = -ry * ff / r;

    a.qx = a.vx;
    a.qy = a.vy;
    a.qz = a.vz;

    a.vx = avx;
    a.vy = avy;
    a.vz = 0;

    return a;
}


