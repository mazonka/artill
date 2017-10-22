#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <cmath>
#include <cstdlib>
#include <cstdio>
#include <random>
#include <algorithm>
#include <iterator>
#include <functional>
#include <iomanip>

using namespace std;

// ***********************************
// Constants
// ***********************************

namespace g
{
const double Earth_radius = 6378137;
const double Earth_mass   = 5.9722e24;
const double Ggrav        = 6.67408e-11;
const double dt0          = 0.1; // initial time step
const int renorm_E        = 10;  // steps between E renormalisation
} //g
static const double PI = acos(-1.0);

// Constants
// ***********************************



// ***********************************
// Types
// ***********************************


struct Fpoint
{
    double t, qx, qy, qz, vx, vy, vz;
    mutable double r1, r2;

    Fpoint(double a, double b, double c, double d, double e)
        : t(a), qx(b), qy(c), qz(0), vx(d), vy(e), vz(0), r1(-1) {}

    double R1() const { if (r1 < 0) calcR(); return r1; }
    double R2() const { if (r1 < 0) calcR(); return r2; }
    void calcR() const;

    Fpoint next(double h) const;

    static Fpoint f(Fpoint a);

    Fpoint operator*=(double d)
    {
        qx *= d; qy *= d; qz *= d; t *= d;
        vx *= d; vy *= d; vz *= d; r1 = -1;
        return *this;
    }

    Fpoint operator+=(const Fpoint & a)
    {
        t += a.t;
        qx += a.qx; qy += a.qy; qz += a.qz;
        vx += a.vx; vy += a.vy; vz += a.vz;
        r1 = -1;
        return *this;
    }

    friend ostream & operator<<(ostream & o, Fpoint a)
    {
        return o << "{" << a.t << ','
               << a.qx << ',' << a.qy << ',' << a.qz << ','
               << a.vx << ',' << a.vy << ',' << a.vz << '}';
    }

    bool operator==(const Fpoint & a) const
    {
        return qx == a.qx && qy == a.qy && qz == a.qz
               && vx == a.vx && vy == a.vy && vz == a.vz;
    }
};

Fpoint operator*(double d, Fpoint a) { return a *= d; }
Fpoint operator+(Fpoint a, const Fpoint & b) { return a += b; }

class Rnd
{
        std::default_random_engine reng;
        std::uniform_real_distribution<double> dist;
        std::function<double()> rnd;

    public:

        static unsigned long seed;

        Rnd(): reng(seed), dist(0, 1), rnd( std::bind(dist, reng) ) {}
        double operator()() { return rnd(); }
};

namespace g
{
extern Rnd rnd01;
}

class Marsaglia
{
        double disp;
        double second;

        static double f(double d, double & t)
        {
            double w1, w2, r = 2;

            while ( r >= 1.0 )
            {
                w1 = 2.0 * g::rnd01() - 1.0;
                w2 = 2.0 * g::rnd01() - 1.0;
                r = w1 * w1 + w2 * w2;
            }

            r = std::sqrt( -2.0 * std::log(r) / r );
            t = w2 * r * d;
            return ( w1 * r * d );
        }

    public:
        Marsaglia(double d): disp(d), second(0) {}

        double operator()()
        {
            if ( second == 0 ) return f(disp, second);
            double t = second; second = 0; return t;
        }
};

class Marsaglia3d
{
    public:
        double x, y, z;
        Marsaglia3d(double d)
        {
            static const double s3 = std::sqrt(3);
            Marsaglia m(d);
            x = m() / s3;
            y = m() / s3;
            z = m() / s3;
        }
};

class Trajectory
{
        vector<Fpoint> path;
        double dt;
        double dEacc;
        double distance;
        int reject;

        int check;
        int renorm;

        double Ek0, Ep0;
        double angle0;

        void cutEnd();
        void smooth();

    public:

        struct Result
        {
            double t; // time
            double r; // range
            double z; // perpendicular deviation
            double rej; // rejection rate // FIXME
            double ek0, ekf, heat, distance;
        };

        Trajectory(int idlt);
        bool evaluate();
        void save();
        Result finalize();
};

struct Drag { double gamma, rho; };

struct Names
{
    map<string, double *> sd;
    map<string, int *> si;

    Names();
    void dump();
};

// Types
// ***********************************



// ***********************************
// Function declarations
// ***********************************

Drag getDrag(const Fpoint & y);
void applyDrag(const Fpoint & y1, Fpoint & y2, Drag);
void applyKick(const Fpoint & y1, Fpoint & y2, Drag);
void tracef(const char * L, const Fpoint & y1, const Fpoint & y2,
            double dt, double dE, double dK, double dP);
double dEp(Fpoint a, Fpoint b);
double dist(Fpoint a, Fpoint b);
double rangeR(Fpoint a);
Fpoint rangeP(Fpoint a);
bool renormE(Fpoint & p, double e);

std::pair<double, double> air_rho(double Z);
double Cd(double, double);

inline double deg2rad(double x) { return x * PI / 180; }
inline double rad2deg(double x) { return x * 180 / PI; }
inline double rad2mil(double x) { return x * 3200 / PI; }

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

// Function declarations
// ***********************************



// ***********************************
// Globals
// ***********************************

unsigned long Rnd::seed = 1;

namespace g
{
Rnd rnd01;
double mass;
int nTraj;

double angle_deg, angle_dlt;
double height;
double velocity;
double diameter;
double length;
double turbulence;
double zwind;
double xwind;
double temperature = 15; // default temperature

double bc0, bc1, bc2, bd1, bd2, bd3, shape;
double maxDist;
double dtmax;

// intergration parameters
double dEpm_max, dEpm_min, dEpm_rej;
double precision;
double dh_up, dh_dn;

double dragcare = 0.5;
} //g

const char output[] = "art.dat";
const char input[] = "art.inp";
const char inpsav[] = "art.sav";
const char outrng[] = "earth.dat";
const char outsum[] = "art.sum"; // FIXME add summary with means and variances

// Globals
// ***********************************



// ***********************************
// Driver
// ***********************************

int main()
try
{
    void init();
    init();

    {
        ofstream in1(output);
        ofstream in2(outrng);
        ofstream in3(outsum);

        in3 <<
            "# ElevMils    Range         Z             Y             "
            "TOF           MaxH          Vfin          AOFdeg        "
            "EK0           EKfin         RejectRate%" << '\n';
    }

    for ( int i = 0; i < g::nTraj; i++ )
    {
        Trajectory t(i);
        while (t.evaluate());
        Trajectory::Result tr = t.finalize();

        cout << "saving " << (i + 1) << '/' << g::nTraj << ' ' << flush;
        t.save();
        cout << "ok\n";
    }

}
catch (string e)
{
    cout << "Error: " << e << '\n';
    return 1;
}
catch (exception & e)
{
    cout << "Error: " << e.what() << '\n';
    return 2;
}
catch (...)
{
    cout << "Exception caught\n";
    return 3;
}

void init()
{
    map<string, string> m;

    {
        ifstream in(input);
        string k, v;
        for (; in >> k; )
        {
            if ( k == "#" ) { getline(in, v); continue; }
            in >> v;
            if ( !k.empty() && k[0] != '#' ) m[k] = v;
        }
    }

    while (!m.empty())
    {
        auto kv = *m.begin();

        bool recoginse(pair<string, string>);
        if ( !recoginse(kv) ) throw "Unknown input [" + kv.first + "]";

        m.erase(m.begin());
    }

    const double & R = g::Earth_radius;
    double gacc = g::Ggrav * g::Earth_mass / (R * R);
    cout << "Grav g = " << gacc << '\n';

    Names().dump();
}

Names::Names()
{
    sd["dtmax"] = &g::dtmax;
    sd["angle_deg"] = &g::angle_deg;
    sd["angle_dlt"] = &g::angle_dlt;
    sd["diameter"] = &g::diameter;
    sd["length"] = &g::length;
    sd["turbulence"] = &g::turbulence;
    sd["zwind"] = &g::zwind;
    sd["xwind"] = &g::xwind;
    sd["temperature"] = &g::temperature;
    sd["height"] = &g::height;
    sd["mass"] = &g::mass;
    sd["velocity"] = &g::velocity;
    sd["shape"] = &g::shape;
    sd["bc0"] = &g::bc0;
    sd["bc1"] = &g::bc1;
    sd["bc2"] = &g::bc2;
    sd["bd1"] = &g::bd1;
    sd["bd2"] = &g::bd2;
    sd["bd3"] = &g::bd3;
    sd["max_dist"] = &g::maxDist;
    sd["dEpm_rej"] = &g::dEpm_rej;
    sd["dEpm_max"] = &g::dEpm_max;
    sd["dEpm_min"] = &g::dEpm_min;
    sd["dh_up"] = &g::dh_up;
    sd["dh_dn"] = &g::dh_dn;
    sd["precision"] = &g::precision;

    si["ntraj"] = &g::nTraj;
}

void Names::dump()
{
    ofstream of(inpsav);

    auto s = [](string x) -> string
    {
        int z = 20 - x.size();
        if (z <= 0) z = 1;
        return string(z, ' ');
    };

    for ( auto x : sd )
    {
        string n = x.first;
        of << n << s(n) << (*x.second) << '\n';
    }

    for ( auto x : si )
    {
        string n = x.first;
        of << n << s(n) << (*x.second) << '\n';
    }
}

bool recoginse(pair<string, string> kv)
{
    string k = kv.first;
    string v = kv.second;
    double d = atof(v.c_str());
    int i = atoi(v.c_str());

    static Names names;

    if ( names.sd.find(k) != names.sd.end() ) *names.sd[k] = d;
    else if ( names.si.find(k) != names.si.end() ) *names.si[k] = i;
    else return false;

    return true;
}

// Driver
// ***********************************


// ***********************************
// Trajectory
// ***********************************

Trajectory::Trajectory(int idlt)
{
    angle0 = deg2rad(g::angle_deg + idlt * g::angle_dlt);
    double vx = g::velocity * cos(angle0);
    double vy = g::velocity * sin(angle0);

    Fpoint p(0, 0, g::height, vx, vy);

    path.clear();
    path.push_back(p);

    dt = g::dt0;
    dEacc = 0;
    distance = 0;
    reject = 0;

    renorm = g::renorm_E;

    Fpoint z(0, 0, 0, 0, 0);
    Ek0 = dEk(z, p);
    Ep0 = dEp(z, p);
}

void Trajectory::save()
{
    auto s = [](double x)->string
    {
        const int SZ = 13;
        ostringstream os; os << x;
        string q = os.str();
        int z = q.size();
        if ( z < SZ ) q += string(SZ - z, ' ');
        return q;
    };


    Fpoint last = path[path.size() - 1];
    double range = rangeR(last);

    //if( int(range+0.5)%100 ) return;

    {
        ofstream rg(outrng, ios::app);
        ofstream of(output, ios::app);

        of <<
           "# Time        X             Y             Z             "
           "vX            vY            vZ            dt            "
           "Distance      Range         HeatKJ"
           << '\n';

        Fpoint z = path[0];

        auto heat = [z](Fpoint w)->double { return dEk(w, z) + dEp(w, z); };

        Fpoint q = z;
        double S = 0;
        for ( auto p : path )
        {
            double dt = p.t - q.t;
            S += dist(p, q);

            of << s(p.t)
               << ' ' << s(p.qx) << ' ' << s(p.qy) << ' ' << s(p.qz)
               << ' ' << s(p.vx) << ' ' << s(p.vy) << ' ' << s(p.vz)
               << ' ' << s(dt) << ' ' << s(S)
               << ' ' << s(rangeR(p)) << ' ' << s(heat(p) / 1000)
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
        double h = p.R1() - g::Earth_radius;
        double y = p.qy;
        if ( maxH < h ) maxH = h;
        if ( maxY < y ) maxY = y;
    }

    {
        ofstream sm(outsum, ios::app);

        sm << s(rad2mil(angle0));
        sm << ' ' << s(range);
        sm << ' ' << s(last.qz) << ' ' << s(last.qy);
        sm << ' ' << s(last.t) << ' ' << s(maxH);
        sm << ' ' << s(sqrt(2 * Ek(last))) << ' ' << s(rad2deg(atan2(-last.vy, last.vx)));
        sm << ' ' << s(Ek0 * g::mass) << ' ' << s(g::mass * Ek(last));
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

    const double & R = g::Earth_radius;

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

Trajectory::Result Trajectory::finalize()
{
    cutEnd();
    smooth();

    Fpoint last = path[path.size() - 1];

    Result x;
    x.r = rangeR(last);
    x.z = last.qz;
    x.t = last.t;

    return x;
}

// Trajectory
// ***********************************



// ***********************************
// Integrator
// ***********************************

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

    const Fpoint & y1 = path[path.size() - 1];
    Fpoint y2(0, 0, 0, 0, 0);
    double dE, dK, dP;

    //double Esca = Ek(y1)+dEp(y2,y1);
    double Esca = Ep0 + Ek0;
    double demin = g::dEpm_min * g::precision * Esca;
    double demax = g::dEpm_max * g::precision * Esca;
    double derej = g::dEpm_rej * g::precision * Esca;

    if ( g::dtmax > 0 && g::dtmax < dt ) dt = g::dtmax;

    Drag drag = getDrag(y1);
    while ( drag.gamma * dt > g::dragcare ) dt *= g::dh_dn;

    for ( int i = 0; i < 1000; i++ )
    {
        y2 = y1.next(dt);

        dK = dEk(y1, y2);
        dP = dEp(y1, y2);
        dE = dK + dP;
        double adE = abs(dE);
        if ( adE < demin ) dt *= g::dh_up;
        if ( adE > demax ) dt *= g::dh_dn;
        if ( adE < derej ) goto accept;

        dt *= g::dh_dn;
        ++reject;

        if (trace) tracef("R", y1, y2, dt, dE, dK, dP);
    }

    cout << y1 << ' ' << y2 << '\n';
    cout << dE << ' ' << dt << '\n';
    throw string() + "No convergence: too low precision";

accept:
    if (trace) tracef("R", y1, y2, dt, dE, dK, dP);

    distance += dist(y1, y2);

    if ( y1 == y2 )
        throw string() + "Dynamic stalled - increase precision parameter";

    dEacc += dE;
    if ( --renorm < 0 && renormE(y2, -dEacc) )
    {
        renorm = g::renorm_E;
        dEacc = 0;
    }

    applyDrag(y1, y2, drag);
    applyKick(y1, y2, drag);

    try
    {
        path.push_back(y2); // this invalidates y1
        if ( path.size() >= 300000 )
            throw string("Trajectory size limit reached");
    }
    catch (...)
    {
        printf("Possible memory allocation fails\n");
        printf("Trajectory size %ld is too big\n", (unsigned)path.size());
        printf("Increase precision parameter\n");
        throw;
    }

    if ( y2.R1() < g::Earth_radius ) return false;
    if ( distance > g::maxDist ) return false;

    return true;
}

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
    static const double GM = g::Ggrav * g::Earth_mass;

    double ry = g::Earth_radius + a.qy;

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

// Integrator
// ***********************************



// ***********************************
// Drag
// ***********************************

Drag getDrag(const Fpoint & y)
{
    // Fd = (1/m)(1/2)v2*rho*A*Cd(v/vs)
    // Fd = Ek*rho*A2m*Cd(v/vs)
    // gamma = Fd/v
    Drag drag;

    static const double A2m = 0.25 * g::diameter * g::diameter * PI / g::mass;
    static const double B2m = g::length * g::diameter / g::mass;

    double e0 = Ek(y);
    Fpoint z = y + Fpoint(0, 0, 0, g::xwind, 0);
    double e1 = Ek(z);
    auto rhovs = air_rho(y.R1() - g::Earth_radius);
    double v1 = sqrt(2 * e1);
    double cd = Cd(v1, rhovs.second);

    double C2m = A2m;
    const double smll = 1e-6;

    // if e0=0 orientation of projectile is not known
    // if e1=0 => gamma is small anyway
    if ( e1 > smll && e0 > smll )
    {
        // C = LDsin+PiR2cos
        // w2 = v02 + v12 - 2v0 v1 cos
        const double w = g::xwind;
        double cos2 = (2 * e0 + 2 * e1 - w * w);
        cos2 *= cos2;
        cos2 /= (16 * e0 * e1);
        C2m = B2m * sqrt(1 - cos2) + A2m * sqrt(cos2);
    }

    double Fdm = rhovs.first * C2m * cd;

    drag.gamma = Fdm * v1;
    drag.rho = rhovs.first;

    return drag;
}

void applyDrag(const Fpoint & y1, Fpoint & y2, Drag drag)
{
    double gm = drag.gamma;

    double dt = y2.t - y1.t;

    double ex = exp(-dt * drag.gamma);

    y2.vx += g::xwind;
    y2.vx *= ex;
    y2.vx -= g::xwind;

    y2.vy *= ex;
    y2.vz *= ex;

    static const double B2m = g::zwind * g::zwind // CD=1
                              * g::length * g::diameter / g::mass;

    y2.vz += B2m * drag.rho * dt;
}

std::pair<double, double> air_rho(double Z)
{
    Z /= 1000; // to km

    const double P0 = 101325;    // sea level standard pressure, Pa
    const double L = 6.5;        // temperature lapse rate, K/km
    const double E = 6356.766;   // the radius of the earth, km
    const double T0 = 288.15;    // sea level standard temperature, K
    const double g = 9.80665;    // gravitational constant, m/sec2
    const double M = 28.9644;    // molecular weight of dry air, gm/mol
    const double R = 8.31432;    // gas constant, J/(mol*K)
    const double v0 = 20.046;    // speed of sound at 1K

    double H = E * Z / (E + Z);
    double T = T0 - L * H;
    if ( T <= 0 ) return std::make_pair(0., 0.);

    double P = P0 * std::pow(T / T0, g * M / (R * L));

    double rho = P * M / (1000 * R * T);
    double vs = v0 * sqrt(T);
    return std::make_pair(rho, vs);
}

double Cd(double v, double vs)
{
    static const double c0 = g::bc0 * g::shape;
    static const double c1 = g::bc1 * g::shape;
    static const double c2 = g::bc2 * g::shape;
    static const double d1 = g::bd1;
    static const double d2 = g::bd2;
    static const double d3 = g::bd3;

    if ( vs < 1e-6 ) return c0;
    double mu = v / vs;

    auto s = [](double x)->double { return 3 * x * x - 2 * x * x * x; };

    if ( mu < d1 ) return c0;
    if ( mu < d2 ) return c0 + (c1 - c0) * s((mu - d1) / (d2 - d1));
    if ( mu < d3 ) return c1 + (c2 - c1) * s((mu - d2) / (d3 - d2));

    return c2;
}

void applyKick(const Fpoint & y1, Fpoint & y2, Drag drag)
{
    // <RR>t = 2*gamma*T
    // <rr> = 2*gamma*E/m

    double sigmar = sqrt(drag.gamma) * g::turbulence;
    double sdt = sqrt(y2.t - y1.t);

    Marsaglia3d w(sigmar);

    y2.vx += w.x * sdt;
    y2.vy += w.y * sdt;
    y2.vz += w.z * sdt;
}

// Drag
// ***********************************


// ***********************************
// Utilities
// ***********************************

double dEp(Fpoint a, Fpoint b)
{
    // ignore z coordinate

    static const double GM = g::Ggrav * g::Earth_mass;

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

    double nom = 2 * g::Earth_radius * ym + ym * yp + xm * xp;
    double den = ra * rb * (ra + rb);
    return GM * nom / den;
}

double dist(Fpoint a, Fpoint b)
{
    double x = a.qx - b.qx;
    double y = a.qy - b.qy;
    double z = a.qz - b.qz;
    double r = sqrt(x * x + y * y + z * z);
    return r;
}

double rangeR(Fpoint a)
{
    const double & R = g::Earth_radius;
    double theta = atan2(a.qx, R + a.qy);

    // the range can be negative because of the front wind
    if (0) if (theta < 0 ) theta = 2 * PI - theta;

    return R * theta;
}

Fpoint rangeP(Fpoint a)
{
    const double & R = g::Earth_radius;
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

    double v = sqrt(v2);
    double D = v2 + 2 * e;
    if ( D < 0 ) return false;
    double x = -v + sqrt(D);

    p.vx += x * p.vx / v;
    p.vy += x * p.vy / v;
    p.vz += x * p.vz / v;

    return true;
}

void tracef(const char * L, const Fpoint & y1, const Fpoint & y2,
            double dt, double dE, double dK, double dP)
{
    auto o = std::setprecision(20);
    cout << o << L << ": " << y1 << ' ' << y2 << '\n';
    cout << o << "\tdt=" << dt << " dE=" << dE << " K/P=" << dK << '/' << dP << '\n';
}

void Fpoint::calcR() const
{
    double ry = g::Earth_radius + qy;
    r2 = ry * ry + qx * qx;
    r1 = sqrt(r2);
}

// Utilities
// ***********************************
