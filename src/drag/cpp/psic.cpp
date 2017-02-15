#include "psic.h"

#include "defs.h"
#include "names.h"
#include "cd.h"

double cst::Earth_radius = 6378137;
double cst::Earth_mass = 5.9722e24;
double cst::Ggrav = 6.67408e-11;
double cst::StdPress = 101325;
double cst::TempRate = 6.5;
double cst::EarthAtmR = 6356.766;
double cst::StdTemp = 288.15;
double cst::GravAtm = 9.80665;
double cst::MolecW = 28.9644;
double cst::GasConst = 8.31432;
double cst::Sound1K = 20.046;

Psi::~Psi()
{
    delete cd;
}

Psi::Psi(): cd(nullptr)
{
    s.ntraj = 1;

    // Projectile
    {
        Names nm(proj_in);

        nm.sd["diameter"] = &p.diameter;
        nm.sd["length"] = &p.length;
        nm.sd["mass"] = &p.mass;
        nm.si["cdtype"] = &p.cdtype;
        nm.sd["velocity"] = &p.velocity;

        nm.load(p.NP);

        nm.save(proj_out);
    }

    // Shot
    {
        Names nm(shot_in);

        nm.sd["angle_deg"] = &s.angle_deg;
        nm.sd["angle_dlt"] = &s.angle_dlt;
        nm.sd["turbulence"] = &s.turbulence;
        nm.sd["zwind"] = &s.zwind;
        nm.sd["xwind"] = &s.xwind;
        nm.sd["height"] = &s.height;
        nm.si["ntraj"] = &s.ntraj;

        nm.load(s.NS);

        nm.save(shot_out);
    }

    // Integrate
    {
        Names nm(integr_in);

        nm.sd["dt0"] = &i.dt0;
        nm.sd["dtmax"] = &i.dtmax;
        nm.sd["max_len"] = &i.maxLength;
        nm.sd["dEpm_rej"] = &i.dEpm_rej;
        nm.sd["dEpm_max"] = &i.dEpm_max;
        nm.sd["dEpm_min"] = &i.dEpm_min;
        nm.sd["dh_up"] = &i.dh_up;
        nm.sd["dh_dn"] = &i.dh_dn;
        nm.sd["precision"] = &i.precision;
        nm.si["renormE"] = &i.renormE;
        nm.sd["dragcare"] = &i.dragcare;

        nm.load(i.NI);

        nm.save(integr_out);
    }

    // Constants
    {
        Names nm(consts_in);

        nm.sd["Earth_radius"] = &cst::Earth_radius;
        nm.sd["Earth_mass"] = &cst::Earth_mass;
        nm.sd["Ggrav"] = &cst::Ggrav;
        nm.sd["std_press"] = &cst::StdPress;
        nm.sd["temp_rate"] = &cst::TempRate;
        nm.sd["Earth_atm_R"] = &cst::EarthAtmR;
        nm.sd["std_temp"] = &cst::StdTemp;
        nm.sd["grav_atm"] = &cst::GravAtm;
        nm.sd["molec_w"] = &cst::MolecW;
        nm.sd["gas_const"] = &cst::GasConst;
        nm.sd["speed_1k"] = &cst::Sound1K;

        nm.load(cst::NC);

        nm.save(consts_out);
    }

    cd = Cd::factory(p.cdtype);
}

Psi::Psi(const Psi & x): p(x.p), s(x.s), i(x.i)
{
    cd = x.cd->clone();
}
