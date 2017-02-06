#include <sstream>
#include <fstream>
#include <iostream>
#include <set>
#include <cstdio>
#include <cmath>

#include "util.h"
#include "physics.h"
#include "traj.h"

Trajectory::Trajectory(const Psi * apsi, int idlt): psi(apsi)
{
    double s_angle_deg = psi->s.angle_deg;
    double s_angle_dlt = psi->s.angle_dlt;
    double p_velocity = psi->p.velocity;

    angle0 = deg2rad(s_angle_deg + idlt * s_angle_dlt);
    double vx = p_velocity * cos(angle0);
    double vy = p_velocity * sin(angle0);

    Fpoint p(0, 0, psi->s.height, vx, vy);

    path.clear();
    path.push_back(p);

    dt = psi->i.dt0;
    dEacc = 0;
    trlength = 0;
    reject = 0;

    renorm = psi->i.renormE;

    Fpoint z(0, 0, 0, 0, 0);
    Ek0 = dEk(z, p);
    Ep0 = dEp(z, p);
}

void Trajectory::save()
{
    auto s = [](double x)->string { return format(x); };

    Fpoint last = path[path.size() - 1];
    double range = rangeR(last);
    double mass = psi->p.mass;

    //if( int(range+0.5)%100 ) return;

    {
        std::ofstream rg(earth_out, std::ios::app);
        std::ofstream of(trajs_out, std::ios::app);

        of <<
           "# Time        X             Y             Z             "
           "vX            vY            vZ            dt            "
           "Length        Range         HeatKJ        Mach"
           << '\n';

        Fpoint z = path[0];

        auto heat = [z, mass](Fpoint w)->double
        {
            return mass * (dEk(w, z) + dEp(w, z));
        };

        Fpoint q = z;
        double S = 0;
        for ( auto p : path )
        {
            double du = p.t - q.t;
            S += dist(p, q);

            of << s(p.t)
               << ' ' << s(p.qx) << ' ' << s(p.qy) << ' ' << s(p.qz)
               << ' ' << s(p.vx) << ' ' << s(p.vy) << ' ' << s(p.vz)
               << ' ' << s(du) << ' ' << s(S)
               << ' ' << s(rangeR(p)) << ' ' << s(heat(p) / 1000)
               << ' ' << s(mach(p))
               << '\n';

            Fpoint r = rangeP(p);
            rg << s(r.qx) << ' ' << s(r.qy) << ' ' << s(r.qz) << '\n';

            q = p;
        }

        of << '\n';
        rg << '\n';
    }

    // claculate max height
    double maxH = 0, maxY = 0;
    for ( auto p : path )
    {
        double h = p.R1() - cst::Earth_radius;
        double y = p.qy;
        if ( maxH < h ) maxH = h;
        if ( maxY < y ) maxY = y;
    }

    {
        std::ofstream sm(summary_out, std::ios::app);

        sm << s(rad2mil(angle0));
        sm << ' ' << s(range);
        sm << ' ' << s(last.qz) << ' ' << s(last.qy);
        sm << ' ' << s(last.t) << ' ' << s(maxH);
        sm << ' ' << s(sqrt(2 * Ek(last)));
        sm << ' ' << s(r2d(atan2(-last.vy, last.vx)));
        sm << ' ' << s(Ek0 * mass) << ' ' << s(mass * Ek(last));
        sm << ' ' << s(int(0.5 + reject * 100.0 / path.size()));
        sm << '\n';
    }
}

void Trajectory::smooth()
{
    const int NK = 2; // number of iterations

    static const double tolerance = 5 * PI / 180; // angle tolerance

    int sz = path.size();
    if ( sz < 3 ) return;

    for ( int k = 0; k < NK; k++ )
    {
        std::set<int> v;
        sz = path.size(); // path may be changed

        for ( int i = 2; i < sz; i++ )
        {
            Fpoint y1 = path[i - 2];
            Fpoint y2 = path[i - 1];
            Fpoint y3 = path[i];

            Fpoint y12 = y2 + (-1) * y1;
            Fpoint y23 = y3 + (-1) * y2;

            double a12 = y12.qx * y12.qx + y12.qy * y12.qy + y12.qz * y12.qz;
            double a23 = y23.qx * y23.qx + y23.qy * y23.qy + y23.qz * y23.qz;
            double dot = y12.qx * y23.qx + y12.qy * y23.qy + y12.qz * y23.qz;

            double a = a12 * a23;
            if ( a < 1e-6 ) continue;

            double ang = acos(dot / sqrt(a));
            if ( ang < tolerance ) continue;

            v.insert(i - 1);
            v.insert(i);
        }

        if ( v.empty() ) break;

        std::vector<Fpoint> newpath;
        newpath.push_back(path[0]);

        for ( int i = 1; i < sz; i++ )
        {
            Fpoint y1 = path[i - 1];
            Fpoint y2 = path[i];
            Fpoint y0 = y1;
            Fpoint y3 = y2;

            if ( i > 1 ) y0 = path[i - 2];
            if ( i < sz - 1 ) y3 = path[i + 1];

            if ( v.find(i) != v.end() )
            {
                Fpoint z0 = 0.5 * (y1 + y2);
                Fpoint z1 = y1 + 0.5 * (y2 + (-1) * y0);
                Fpoint z2 = y2 + 0.5 * (y1 + (-1) * y3);
                Fpoint z4 = (1. / 6) * ( z1 + z2 + 4 * z0);
                newpath.push_back(z4);
            }

            newpath.push_back(y2);
        }
        path.swap(newpath);

    } // next it

}

void Trajectory::cutEnd()
{
    if ( path.empty() ) return;

    const double & R = cst::Earth_radius;

    while (path.size() > 3 && path[path.size() - 2].R1() < R )
        path.pop_back();

    int sz = path.size();

    Fpoint & a = path[sz - 2];
    Fpoint & b = path[sz - 1];
    double ra = a.R1();
    double rb = b.R1();

    if ( rb > R || ra < R )
        return;

    double e = dEkp(a, b);

    double c = (R - ra) / (rb - ra);

    b.t = a.t + (b.t - a.t) * c;

    b.qx = a.qx + (b.qx - a.qx) * c;
    b.qy = a.qy + (b.qy - a.qy) * c;
    b.qz = a.qz + (b.qz - a.qz) * c;

    b.vx = a.vx + (b.vx - a.vx) * c;
    b.vy = a.vy + (b.vy - a.vy) * c;
    b.vz = a.vz + (b.vz - a.vz) * c;

    b.calcR();

    renormE(b, e * c - dEkp(a, b));
}

TrResult Trajectory::finalize()
{
    cutEnd();
    smooth();

    TrResult x(psi, path);
    fillResult(x);

    return x;
}


inline bool testOneDown(int sz, const Fpoint & y1, const Fpoint & y2, double ER)
{
    if (sz > 1) return true;
    bool goingDown = (y2.R1() < y1.R1());
    bool belowEarth = (y2.R1() < ER);
    return !(goingDown && belowEarth);
}

bool Trajectory::evaluate()
{
    const bool trace = false;

    // calc next point
    // accept-reject
    // check dist - add dS
    // check renorm - add dE
    // apply drag
    // apply kick
    // add point

    const int sz = path.size();
    const double ER = cst::Earth_radius;
    const IntgBlob & pi = psi->i;

    const Fpoint & y1 = path[sz - 1];
    Fpoint y2(0, 0, 0, 0, 0);
    double dE = 0, dK, dP;

    //double Esca = Ek(y1)+dEp(y2,y1);
    double Esca = Ep0 + Ek0;
    double demin = pi.dEpm_min * pi.precision * Esca;
    double demax = pi.dEpm_max * pi.precision * Esca;
    double derej = pi.dEpm_rej * pi.precision * Esca;

    if ( pi.dtmax > 0 && pi.dtmax < dt ) dt = pi.dtmax;

    Drag drag = getDrag(psi, y1);
    while ( drag.gamma * dt > pi.dragcare ) dt *= pi.dh_dn;

    for ( int i = 0; i < 1000; i++ )
    {
        y2 = y1.next(dt);

        dK = dEk(y1, y2);
        dP = dEp(y1, y2);
        dE = dK + dP;

        double adE = std::abs(dE);

        if (testOneDown(sz, y1, y2, ER))
        {
            if ( adE < demin ) dt *= pi.dh_up;
            if ( adE > demax ) dt *= pi.dh_dn;
            if ( adE < derej ) goto accept;
        }

        dt *= pi.dh_dn;
        ++reject;

        if (trace) tracef("R", y1, y2, dt, dE, dK, dP);
    }

    std::cout << "y1=" << y1 << " y2=" << y2 << '\n';
    std::cout << "dE=" << dE << " dt=" << dt << '\n';
    throw string() + "No convergence: too low precision";

accept:
    if (trace) tracef("R", y1, y2, dt, dE, dK, dP);

    trlength += dist(y1, y2);

    if ( y1 == y2 )
        throw string() + "Dynamic stalled - increase precision parameter";

    dEacc += dE;
    if ( --renorm < 0 && renormE(y2, -dEacc) )
    {
        renorm = psi->i.renormE;
        dEacc = 0;
    }

    applyDrag(psi, y1, y2, drag);
    applyKick(psi, y1, y2, drag);

    bool goingDown = (y2.R1() < y1.R1()); // check here 'cos y1 invalid later

    try
    {
        path.push_back(y2); // this invalidates y1
        if ( path.size() >= 300000 )
            throw string("Trajectory size limit reached");
    }
    catch (...)
    {
        std::printf("Possible memory allocation fails\n");
        std::printf("Trajectory size %ld is too big\n", (unsigned long)path.size());
        std::printf("Increase precision parameter\n");
        throw;
    }

    bool belowEarth = (y2.R1() < ER);
    if ( ( goingDown && belowEarth ) ||  trlength > pi.maxLength )
    {
        renormE(y2, -dEacc);
        return false;
    }

    return true;
}

void Trajectory::fillResult(TrResult & res)
{
    res.alpha = angle0;
    res.length = trlength;
    res.rej = 1.0 * reject / path.size();
}


TrResult Trajectory::run(const Psi * psi)
{
    Trajectory t(psi, 0);
    while (t.evaluate());
    return t.finalize();
}

double Trajectory::mach(Fpoint y)
{
    Fpoint z = y + Fpoint(0, 0, 0, psi->s.xwind, 0);
    double e1 = Ek(z);
    auto rhovs = air_rho(y.R1() - cst::Earth_radius);
    double v1 = std::sqrt(2 * e1);
    if ( rhovs.second <= 1e-10 ) return 0;
    return v1 / rhovs.second;
}
