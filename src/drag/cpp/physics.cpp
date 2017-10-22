#include <iomanip>
#include <iostream>
#include <cmath>

#include "util.h"
#include "psic.h"
#include "physics.h"
#include "cd.h"
#include "rnd.h"

double dEp(Fpoint a, Fpoint b)
{
    // ignore z coordinate

    static const double GM = cst::Ggrav * cst::Earth_mass;

    // this naive formula is no good, because of computational errors
    if (0) return GM * ( 1 / a.R1() - 1 / b.R1() );

    // ( 1 / ra - 1 / rb ) = (rb2-ra2)/(ra*rb*(ra+rb))
    // (rb2-ra2) = 2R*(yb-ya)+yb2-ya2+xb2-xa2
    double ra = a.R1();
    double rb = b.R1();
    double xm = b.qx - a.qx;
    double xp = b.qx + a.qx;
    double ym = b.qy - a.qy;
    double yp = b.qy + a.qy;

    double nom = 2 * cst::Earth_radius * ym + ym * yp + xm * xp;
    double den = ra * rb * (ra + rb);
    return GM * nom / den;
}

double dist(Fpoint a, Fpoint b)
{
    double x = a.qx - b.qx;
    double y = a.qy - b.qy;
    double z = a.qz - b.qz;
    double r = std::sqrt(x * x + y * y + z * z);
    return r;
}

double rangeR(Fpoint a)
{
    const double & R = cst::Earth_radius;
    double theta = std::atan2(a.qx, R + a.qy);

    // the range can be negative because of the front wind
    if (0) if (theta < 0 ) theta = 2 * PI - theta;

    return R * theta;
}

Fpoint rangeP(Fpoint a)
{
    const double & R = cst::Earth_radius;
    Fpoint p = a;
    double r = a.R1();
    p.qx = R * a.qx / r;
    p.qy = R * (R + a.qy - r) / r;
    return p;
}

bool renormE(Fpoint & p, double e)
{
    // dE = ((v+x)2-v2)/2
    // x2/2+vx-dE=0
    // x = -v + sqrt(v2+2dE)
    // dvx = x*vx/v
    // dvy = x*vy/v

    double v2 = p.vx * p.vx + p.vy * p.vy + p.vz * p.vz;
    if ( v2 < 1e-8 ) return false;

    double v = std::sqrt(v2);
    double D = v2 + 2 * e;
    if ( D < 0 ) return false;
    double x = -v + std::sqrt(D);

    if (x == 0) return false;

    p.vx += x * p.vx / v;
    p.vy += x * p.vy / v;
    p.vz += x * p.vz / v;

    return true;
}

void tracef(const char * L, const Fpoint & y1, const Fpoint & y2,
            double dt, double dE, double dK, double dP)
{
    auto o = std::setprecision(20);
    std::cout << o << L << ": " << y1 << ' ' << y2 << '\n';
    std::cout << o << "\tdt=" << dt << " dE=" << dE << " K/P=" << dK << '/' << dP << '\n';
}

void Fpoint::calcR() const
{
    double ry = cst::Earth_radius + qy;
    r2 = ry * ry + qx * qx;
    r1 = std::sqrt(r2);
}

Drag getDrag(const Psi * psi, const Fpoint & y)
{
    // Fd = (1/m)(1/2)v2*rho*A*Cd(v/vs)
    // Fd = Ek*rho*A2m*Cd(v/vs)
    // gamma = Fd/v
    Drag drag;

    const double A2m = 0.25 * psi->p.diameter * psi->p.diameter * PI / psi->p.mass;
    const double B2m = psi->p.length * psi->p.diameter / psi->p.mass;

    double e0 = Ek(y);
    Fpoint z = y + Fpoint(0, 0, 0, psi->s.xwind, 0);
    double e1 = Ek(z);
    auto rhovs = air_rho(y.R1() - cst::Earth_radius);
    double v1 = std::sqrt(2 * e1);
    double cd = psi->cd->calc(v1, rhovs.second);

    if ( cd < 0 )
    {
        O(v1) O(rhovs.second) O(cd)
        psi->cd->saveall();
        never("cd");
    }

    double C2m = A2m;
    const double smll = 1e-6;

    // if e0=0 orientation of projectile is not known
    // if e1=0 => gamma is small anyway
    if ( e1 > smll && e0 > smll )
    {
        // C = LDsin+PiR2cos
        // w2 = v02 + v12 - 2v0 v1 cos
        const double w = psi->s.xwind;
        double cos2 = (2 * e0 + 2 * e1 - w * w);
        cos2 *= cos2;
        cos2 /= (16 * e0 * e1);
        if ( cos2 > 1 ) cos2 = 1;
        if ( cos2 < 0 ) cos2 = 0;
        C2m = B2m * std::sqrt(1 - cos2) + A2m * std::sqrt(cos2);
    }

    double Fdm = 0.5 * rhovs.first * C2m * cd; // Cd defined with 1/2

    drag.gamma = Fdm * v1; // v^dot = -gamma*v -> no v^2
    drag.rho = rhovs.first;

    return drag;
}

void applyDrag(const Psi * psi, const Fpoint & y1, Fpoint & y2, Drag drag)
{
    double dt = y2.t - y1.t;

    double ex = std::exp(-dt * drag.gamma);

    y2.vx += psi->s.xwind;
    y2.vx *= ex;
    y2.vx -= psi->s.xwind;

    y2.vy *= ex;
    y2.vz *= ex;

    const double B2m = psi->s.zwind * psi->s.zwind // CD=1
                       * psi->p.length * psi->p.diameter / psi->p.mass;

    y2.vz += B2m * drag.rho * dt;
}

void applyKick(const Psi * psi, const Fpoint & y1, Fpoint & y2, Drag drag)
{
    if ( drag.gamma < 0 ) never("gamma");

    // <RR>t = 2*gamma*T
    // <rr> = 2*gamma*E/m

    double sigmar = std::sqrt(drag.gamma) * psi->s.turbulence;
    double sdt = y2.t - y1.t;
    if ( sdt < 0 ) never("dt");
    sdt = std::sqrt(sdt);

    Marsaglia3d w(sigmar);

    y2.vx += w.x * sdt;
    y2.vy += w.y * sdt;
    y2.vz += w.z * sdt;
}

std::pair<double, double> air_rho(double Z)
{
    Z /= 1000; // to km

    const double P0 = cst::StdPress;   // sea level standard pressure, Pa
    const double L = cst::TempRate;    // temperature lapse rate, K/km
    const double E = cst::EarthAtmR;   // the radius of the earth, km
    const double T0 = cst::StdTemp;    // sea level standard temperature, K
    const double g = cst::GravAtm;     // gravitational constant, m/sec2
    const double M = cst::MolecW;      // molecular weight of dry air, gm/mol
    const double R = cst::GasConst;    // gas constant, J/(mol*K)
    const double v0 = cst::Sound1K;    // speed of sound at 1K

    double H = E * Z / (E + Z);
    double T = T0 - L * H;
    if ( T <= 0 ) return std::make_pair(0., 0.);

    double P = P0 * std::pow(T / T0, g * M / (R * L));

    double rho = P * M / (1000 * R * T);
    double vs = v0 * sqrt(T);
    return std::make_pair(rho, vs);
}

