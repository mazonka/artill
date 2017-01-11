// Projectile-Shot-Integration-Constants

#pragma once

class Cd;

// Projectile
struct ProjBlob
{
    static const int NP = 5;
    double mass;
    double diameter;
    double length;
    double velocity;
    int cdtype; // 0 - old, 1 - graph, 2 - point, 3 - alpha
};

//Shot
struct ShotBlob
{
    static const int NS = 7;
    int ntraj;
    double angle_deg;
    double angle_dlt;
    double height;
    double turbulence;
    double zwind;
    double xwind;
};

//Integration
struct IntgBlob
{
    static const int NI = 11;
    double maxLength;
    double dt0, dtmax;
    double dEpm_max, dEpm_min, dEpm_rej;
    double precision;
    double dh_up, dh_dn;
    int renormE;  // steps between E renormalisation (10)
    double dragcare; // exponent usage precision (0.5)
};

// Constants
namespace cst
{
const int NC = 11;
extern double Earth_radius;
extern double Earth_mass;
extern double Ggrav;

extern double StdPress;      // sea level standard pressure, Pa
extern double TempRate;      // temperature lapse rate, K/km
extern double EarthAtmR;     // the radius of the earth, km
extern double StdTemp;       // sea level standard temperature, K
extern double GravAtm;       // gravitational constant, m/sec2
extern double MolecW;        // molecular weight of dry air, gm/mol
extern double GasConst;      // gas constant, J/(mol*K)
extern double Sound1K;       // speed of sound at 1K
} // cst

struct Psi
{
        Psi();
        ~Psi();

        ProjBlob p;
        ShotBlob s;
        IntgBlob i;

        Cd * cd;

        Psi(const Psi &);
        Psi * clone() const { return new Psi(*this); }

    private:
        void operator=(const Psi &);

    public:
        Psi * setAlpha(double al) { s.angle_deg = al; return this; }
};

