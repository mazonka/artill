#include <fstream>
#include <cmath>

#include "physics.h"
#include "util.h"
#include "psic.h"
#include "psolver.h"
#include "trres.h"

void TrResult::init()
{
    // set all to undef
    tim = range = ek0 = ekf = heatKJ = length = -1;
    v0 = vf = alpha = beta = height0 = -1;
    topHeight = topRange = finalZ = rej = -1;
}

TrResult::TrResult(const Psi * psi, const std::vector<Fpoint> & path)
{
    init();

    int sz = path.size();

    if ( sz < 1 ) return;

    Fpoint first = path[0];
    Fpoint last = path[sz - 1];
    range = rangeR(last);
    finalZ = last.qz;
    tim = last.t;

    beta = std::atan2(-last.vy, last.vx);
    vf = std::sqrt(2 * Ek(last));

    height0 = psi->s.height;
    v0 = psi->p.velocity;

    double m = psi->p.mass;
    ek0 = Ek(first) * m;
    ekf = Ek(last) * m;
    heatKJ = 0.001 * m * (dEk(last, first) + dEp(last, first));


    // claculate top
    const double & ER = cst::Earth_radius;
    double maxH = -1e100, maxY = 0;
    int idx = -1;
    for ( int i = 0; i < sz; i++ )
    {
        double h = path[i].R1() - ER;
        if ( maxH < h ) { maxH = h; idx = i; }
        double y = path[i].qy;
        if ( maxY < y ) maxY = y;
    }

    topHeight = maxH;
    topRange = rangeR(path[idx]);

    if ( idx > 0 && idx < sz - 1
            && path[idx - 1].R1() < path[idx].R1()
            && path[idx + 1].R1() < path[idx].R1() )
    {
        double r1 = rangeR(path[idx - 1]);
        double r2 = rangeR(path[idx]);
        double r3 = rangeR(path[idx + 1]);
        double h1 = path[idx - 1].R1() - ER;
        double h2 = path[idx].R1() - ER;
        double h3 = path[idx + 1].R1() - ER;
        Box bx(r1, r2, r3, h1, h2, h3);

        double rx, hx;
        if ( bx.apex2(1e-15, rx, hx) && rx >= r1 && rx <= r3 )
        {
            topHeight = hx;
            topRange = rx;
        }
    }

}

void TrResult::save(bool append) const
{
    auto d = [](double x)->string { return format(x); };
    auto s = [](string x)->string { return format(x); };

    if ( !append )
    {
        std::ofstream of(trres_out);
        of << s("# v0") << s("Angle_deg") << s("Range") << s("Time")
           << s("Fall_deg") << s("vFinal") << s("topHeight") << s("topRange")
           << s("Drift") << s("HeatKJ") << s("KinE0") << s("KinEf")
           << s("TrajLength") << s("Elevation0")  << s("RejectRate")
           << '\n';
    }

    std::ofstream of(trres_out, std::ios::app);

    if ( !of ) throw "Canot open " + string(trres_out);

    of << d(v0) << d(r2d(alpha)) << d(range) << d(tim)
       << d(r2d(beta)) << d(vf) << d(topHeight) << d(topRange)
       << d(finalZ) << d(heatKJ) << d(ek0) << d(ekf)
       << d(length) << d(height0)  << d(rej)
       << '\n';
}


string TrType::str() const
{
    switch (type)
    {
        case UNKNOWN: return "Unkn";
        case FLAT: return "Flat";
        case HIGH: return "High";
        case MAX: return "MaxR";
    }
    never("type");
}

