#include <fstream>
#include <cmath>

#include "cd.h"
#include "binom.h"
#include "names.h"
#include "util.h"

Cd * Cd::factory(int t)
{
    switch (t)
    {
            ///case 0: return new CdLiner();
        case 0: return new CdFixed();
        case 1: return new CdGraph();
        case 2: return new CdPoint();
        case 3: return new CdAlpha();
    }

    never("cdtype " + tos(t));
}

double Cd::calc(double v, double vs)
{
    if ( vs < 1e-6 ) return 0;
    double x = (v / vs);

    if ( ua < 0 || x < ua ) ua = x;
    if ( ub < 0 || x > ub ) ub = x;

    double r = calc(x);

    if (0)
    {
        std::ofstream of("cduse.tmp", std::ios::app);
        of << x << '\t' << r << '\n';
    }

    return r;
}


void CdLiner::setnm()
{
    // set/reset nm's pointers
    nm.sd["shape="] = &b.b.shape;
    nm.sd["bc0="] = &b.b.c0;
    nm.sd["bc1="] = &b.b.c1;
    nm.sd["bc2="] = &b.b.c2;
    nm.sd["bd1="] = &b.b.d1;
    nm.sd["bd2="] = &b.b.d2;
    nm.sd["bd3="] = &b.b.d3;
}

CdLiner::CdLiner(): nm(cdliner_in)
{
    setnm();

    nm.load(7);

    saveall();
}

CdLiner::CdLiner(const CdLiner & c): nm(c.nm)
{
    // reset nm's pointers
    setnm();

    b = c.b;
}

void CdLiner::savecd()
{
    nm.save(cdliner_out);
}

double CdLiner::calc(double mu)
{
    if ( b.b.shape <= 0 ) return 0;

    const double c0 = b.b.c0 * b.b.shape;
    const double c1 = b.b.c1 * b.b.shape;
    const double c2 = b.b.c2 * b.b.shape;
    const double d1 = b.b.d1;
    const double d2 = b.b.d2;
    const double d3 = b.b.d3;

    auto s = [](double x)->double { return 3 * x * x - 2 * x * x * x; };

    double ret = 0;
    if (0);
    else if ( mu < d1 ) ret = c0;
    else if ( mu < d2 ) ret = c0 + (c1 - c0) * s((mu - d1) / (d2 - d1));
    else if ( mu < d3 ) ret = c1 + (c2 - c1) * s((mu - d2) / (d3 - d2));
    else ret = c2;

    if ( ret < 0 ) return 0;
    return ret;
}

CdGraph::CdGraph()
{
    std::ifstream in(cdgraph_in);

    if ( !in ) throw string() + "Cannot open " + cdgraph_in;

    string s;
    in >> s;

    in >> a >> b;

    int i;
    for ( double x; in >> i >> x; )
        v.push_back(x);

    if ( v.size() < 2 )
        throw string() + "Too few points in " + cdgraph_in;

    saveall();
}

void CdGraph::savecd()
{
    std::ofstream of(cdgraph_out);

    of << "#range " << a << '\t' << b << '\n';

    for ( size_t i = 0; i < v.size(); i++ )
        of << i << '\t' << v[i] << '\n';

}

void Cd::saveplt(const char file[])
{
    std::ofstream of(file);

    for (double x = 0; x < 4; x += 0.01 )
        of << x << '\t' << calc(x) << '\n';
}

double CdGraph::calc_linear(double x)
{
    auto p = [](double x)->double { return x < 0 ? 0 : x; };

    if ( x < a ) return p(v[0]);
    int sz = v.size();
    if ( x > b ) return p(v[sz - 1]);

    double delt = (b - a) / (sz - 1); // can be cached
    int i = int((x - a) / delt);

    if ( i > sz - 2 ) return p(v[sz - 1]);

    const double & y1 = v[i];
    const double & y2 = v[i + 1];

    double x1 = a + i * delt;
    double x2 = x1 + delt;
    return p(linear(x, x1, x2, y1, y2));
}

double CdGraph::calc_bezier(double x)
{
    auto p = [](double x)->double { return x < 0 ? 0 : x; };

    if ( x < a ) return p(v[0]);
    int sz = v.size();
    if ( x > b ) return p(v[sz - 1]);

    int n = sz - 1; // number of segments
    double t = (x-a)/(b-a);

    double sum = 0;
    for (int i = 0; i <= n; i++ )
    {
        double vy = v[i];
        double t1 = std::pow(1 - t, n - i);
        double t2 = std::pow(t, i);
        sum += binomial(n, i) * t1 * t2 * vy;
    }

    return p(sum);
}

void CdGraph::setParams(const Params & p)
{
    size_t sz = v.size();
    if ( p.v.size() != sz )
        throw "CdGraph::setParams: sizes";

    for ( size_t i = 0; i < sz; i++ )
        v[i] = p.v[i];
}

int CdGraph::sample(int m)
{
    int n = v.size();
    if (m <= 0) return n;
    std::vector<double> nv;

    for (int i = 0; i < m; i++)
        nv.push_back(calc(a + i * (b - a) / (m - 1)));

    v.swap(nv);
    return v.size();
}

CdPointD::CdPointD()
{
    std::ifstream in(cdpoint_in);

    if (!in) throw string() + "Cannot open " + cdpoint_in;

    double z = 0;
    for (double x, y; in >> x >> y;)
    {
        v.push_back(x - z);
        v.push_back(y);
        z = x;
    }

    if (v.size() < 4)
        throw string() + "Too few points in " + cdpoint_in;

    if (v[0] != 0)
        throw string() + "First point in " + cdpoint_in + " must be 0";

    saveall();
}

double CdPointD::calc(double x)
{
    auto p = [](double x)->double { return x < 0 ? 0 : x; };

    int sz = v.size();
    if (sz < 2) throw "size in CdPointD::calc";

    if (x <= v[0]) return p(v[1]);

    double z = 0, x1, x2, y1, y2;
    for (int i = 0; i < sz; i += 2)
    {
        z += v[i];
        if (x > z) continue;
        x2 = z;
        x1 = z - v[i];
        y2 = v[i + 1];
        y1 = v[i - 1];
        return p(linear(x, x1, x2, y1, y2));
    }

    return p(v[sz - 1]);
}

void CdPointD::savecd()
{
    std::ofstream of(cdpoint_out);

    double z = 0;
    for (size_t i = 0; i < v.size(); i += 2)
        of << (z += v[i]) << '\t' << v[i + 1] << '\n';
}

void CdPointD::setParams(const Params & p)
{
    size_t sz = v.size();
    if (p.v.size() + 1 != sz)
        never("sizes");

    for (size_t i = 1; i < sz; i++)
        v[i] = p.v[i - 1];
}

int CdPointD::sample(int m)
{
    if (m <= 0) return v.size() / 2;

    if (v.size() < 4) never(4);

    while ( m > int(v.size() / 2) )
    {
        int sz = v.size();
        std::vector<double> nv;
        double mx = -INF_VAL;
        int idx = -1;
        for (int i = 2; i < sz; i += 2)
        {
            double ln = (v[i + 1] - v[i - 1]);
            ln *= ln;
            ln += v[i] * v[i];
            if (ln > mx)
            {
                mx = ln;
                idx = i;
            }
        }
        for (int i = 0; i < sz; i++)
        {
            if (i != idx)
            {
                nv.push_back(v[i]);
                continue;
            }
            double h1 = v[i] / 2;
            nv.push_back(h1);
            nv.push_back(0.5 * (v[i - 1] + v[i + 1]));
            nv.push_back(v[i] - h1);
        }
        v.swap(nv);
    }

    return v.size() / 2;
}

void CdAlpha::setnm()
{
    // set/reset nm's pointers
    nm.sd["a0="] = &b.b.a0;
    nm.sd["am="] = &b.b.am;
    nm.sd["aw="] = &b.b.aw;
    nm.sd["a1="] = &b.b.a1;
    nm.sd["bm="] = &b.b.bm;
    nm.sd["bw="] = &b.b.bw;
    nm.sd["b1="] = &b.b.b1;
}

void CdAlpha::loadcd(const char * file)
{
    nm.reset(file);
    setnm();
    nm.load(7);
}

CdAlpha::CdAlpha(): nm(cdalpha_in)
{
    setnm();
    nm.load(7);
    saveall();
}

CdAlpha::CdAlpha(const CdAlpha & c): nm(c.nm)
{
    setnm();
    b = c.b;
}

void CdAlpha::savecd() { savecd(cdalpha_out); }
void CdAlpha::savecd(const char * file)
{
    nm.save(file, 20);
}

double CdAlpha::B::calc(double x)
{
    auto p = [](double x)->double { return x < 0 ? 0 : x; };

    using std::exp;

    double waw = aw;
    double wbw = bw;

    static const double smll = 1e-5;
    static const double smll2 = smll * smll;
    if ( waw * waw < smll2 ) waw = smll;
    if ( wbw * wbw < smll2 ) wbw = smll;

    double qa = a0 + a1 / (1 + exp(-(x - am) / waw));
    double qb =  0 + b1 / (1 + exp(-(x - bm) / wbw));

    ///const double smll = 1e-3;
    if ( x < smll ) x = smll;

    return p(qa + qb / x);
}

CdPoint::CdPoint()
{
    std::ifstream in(cdpoint_in);

    if (!in) throw string() + "Cannot open " + cdpoint_in;

    for (double x, y; in >> x >> y;)
    {
        v.push_back(x);
        v.push_back(y);
    }

    if (v.size() < 4)
        throw string() + "Too few points in " + cdpoint_in;

    if (v[0] != 0)
        throw string() + "First point in " + cdpoint_in + " must be 0";

    saveall();
}

double CdPoint::calc_linear(double x)
{
    auto p = [](double x)->double { return x < 0 ? 0 : x; };

    int sz = v.size();
    if (sz < 2) never(0);

    if (x <= v[0]) return p(v[1]);

    double z, x1, x2, y1, y2;
    for (int i = 0; i < sz; i += 2)
    {
        z = v[i];
        if (x > z) continue;
        x2 = z;
        x1 = v[i - 2];
        y2 = v[i + 1];
        y1 = v[i - 1];
        return p(linear(x, x1, x2, y1, y2));
    }

    return p(v[sz - 1]);
}

double CdPoint::calc_bezier(double x)
{
    auto p = [](double x)->double { return x < 0 ? 0 : x; };

    int sz = v.size();
    if (sz < 2) never(0);
    if (sz % 2) never(0);

    double xn = v[0];
    double xx = v[sz - 2];

    if (x <= xn ) return p(v[1]);
    if (x >= xx ) return p(v[sz - 1]);
    int n = sz / 2 - 1; // number of segments
    //double seg = (xx - xn) / n;
    double tseg = 1.0 / n;

    //double t = (xn-x)/(xx-xn); this is not right - it gives equal weights
    double z, x1, x2, t = 0;
    for ( int i = 0; i < sz; i += 2 )
    {
        z = v[i];
        if (x > z) continue;
        x2 = z;
        x1 = v[i - 2];
        //y2 = v[i + 1];
        //y1 = v[i - 1];
        double t0 = (i / 2 - 1) * tseg;
        t = p(linear(x, x1, x2, t0, t0 + tseg));
        break;
    }


    double sum = 0;
    for (int i = 0; i <= n; i++ )
    {
        //double vx = v[2*i];
        double vy = v[2 * i + 1];
        double t1 = std::pow(1 - t, n - i);
        double t2 = std::pow(t, i);
        sum += binomial(n, i) * t1 * t2 * vy;
    }

    return p(sum);
}

void CdPoint::savecd()
{
    std::ofstream of(cdpoint_out);

    for (size_t i = 0; i < v.size(); i += 2)
        of << v[i] << '\t' << v[i + 1] << '\n';
}

void CdPoint::setParams(const Params & p)
{
    size_t sz = v.size();
    if (p.v.size() + 1 != sz)
        never("sizes");

    for (size_t i = 1; i < sz; i++)
        v[i] = p.v[i - 1];
}

int CdPoint::sample(int m)
{
    if (m <= 0) return v.size() / 2;

    if (v.size() < 4) never(4);

    while ( m > int(v.size() / 2) )
    {
        int sz = v.size();
        std::vector<double> nv;
        double mx = -INF_VAL;
        int idx = -1;
        for (int i = 2; i < sz; i += 2)
        {
            double lny = (v[i + 1] - v[i - 1]); lny *= lny;
            double lnx = (v[i] - v[i - 2]); lnx *= lnx;
            double ln = lnx + lny;
            if (ln > mx)
            {
                mx = ln;
                idx = i;
            }
        }
        for (int i = 0; i < sz; i++)
        {
            if (i == idx)
            {
                nv.push_back(0.5 * (v[i] + v[i - 2]));
                nv.push_back(0.5 * (v[i - 1] + v[i + 1]));
            }
            nv.push_back(v[i]);
        }
        v.swap(nv);
    }

    return v.size() / 2;
}

///////////////////////
// Cd Fixed

CdFixed::CdFixed()
{
    std::ifstream in(cdfixed_in);

    if (!in) throw string() + "Cannot open " + cdfixed_in;

    for (double x, y; in >> x >> y;)
    {
        vx.push_back(x);
        vy.push_back(y);
    }

    if (vx.size() < 2)
        throw string() + "Too few points in " + cdfixed_in;

    saveall();
}

double CdFixed::calc(double x)
{
    auto p = [](double x)->double { return x < 0 ? 0 : x; };

    int sz = vx.size();
    if (sz < 2) never(0);

    if (x <= vx[0]) return p(vy[1]);

    double z, x1, x2, y1, y2;
    for (int i = 0; i < sz; i++ )
    {
        z = vx[i];
        if (x > z) continue;
        x2 = z;
        x1 = vx[i - 1];
        y2 = vy[i];
        y1 = vy[i - 1];
        return p(linear(x, x1, x2, y1, y2));
    }

    return p(vy[sz - 1]);
}

void CdFixed::savecd()
{
    std::ofstream of(cdfixed_out);

    for (size_t i = 0; i < vx.size(); i++ )
        of << vx[i] << '\t' << vy[i] << '\n';
}

void CdFixed::setParams(const Params & p)
{
    size_t sz = vy.size();
    if (p.v.size() != sz)
        never("sizes");

    for (size_t i = 0; i < sz; i++)
        vy[i] = p.v[i];
}

